#include "../../include/concurrency/LockManager.h"
#include <iostream>

LockManager::LockManager() : log_manager_("log.txt") {}

LockManager::~LockManager() = default;

void LockManager::abortTransaction(uint32_t txn_id) {
    for (auto& [resource, locks] : resource_locks_) {
        for (auto it = locks.begin(); it != locks.end(); ) {
            if ((*it)->getTransactionId() == txn_id) {
                it = locks.erase(it);
            } else {
                ++it;
            }
        }
    }
}

bool LockManager::requestLock(uint32_t transaction_id, uint32_t resource_id, LockType lock_type) {

    log_manager_.logBegin(transaction_id);

    auto it = resource_locks_.find(resource_id);
    bool conflict = false;

    if (it != resource_locks_.end()) {
        for (const auto& existing_lock : it->second) {

            uint32_t holder_txn = existing_lock->getTransactionId();

            if (holder_txn == transaction_id) continue;

            if (!isCompatible(existing_lock->getLockType(), lock_type)) {

                conflict = true;

                wait_for_graph_.addEdge(transaction_id, holder_txn);

                std::cout << "Transaction " << transaction_id
                          << " is WAITING for txn " << holder_txn << std::endl;
            }
        }
    }

    if (conflict) {

        if (wait_for_graph_.hasCycle()) {

            std::cout << "Deadlock detected! Aborting txn "
                      << transaction_id << std::endl;

            log_manager_.logAbort(transaction_id);

            abortTransaction(transaction_id);

            wait_for_graph_.removeTransaction(transaction_id);
        }

        return false;
    }

    log_manager_.logUpdate(transaction_id, "old_val", "new_val");

    auto new_lock = std::make_shared<Lock>(transaction_id, lock_type);
    resource_locks_[resource_id].insert(new_lock);

    wait_for_graph_.removeTransaction(transaction_id);

    log_manager_.logCommit(transaction_id);

    return true;
}

bool LockManager::releaseLock(uint32_t transaction_id, uint32_t resource_id) {

    auto it = resource_locks_.find(resource_id);
    if (it == resource_locks_.end()) return false;

    auto& locks = it->second;
    bool removed = false;

    for (auto lock_it = locks.begin(); lock_it != locks.end(); ) {
        if ((*lock_it)->getTransactionId() == transaction_id) {
            lock_it = locks.erase(lock_it);
            removed = true;
        } else {
            ++lock_it;
        }
    }

    if (removed) {
        wait_for_graph_.removeTransaction(transaction_id);
    }

    return removed;
}

bool LockManager::hasLock(uint32_t transaction_id, uint32_t resource_id) const {
    auto it = resource_locks_.find(resource_id);

    if (it != resource_locks_.end()) {
        for (const auto& lock : it->second) {
            if (lock->getTransactionId() == transaction_id) {
                return true;
            }
        }
    }
    return false;
}

LockType LockManager::getLockType(uint32_t resource_id) const {
    auto it = resource_locks_.find(resource_id);

    if (it == resource_locks_.end() || it->second.empty()) {
        return LockType::SHARED;
    }

    for (const auto& lock : it->second) {
        if (lock->getLockType() == LockType::EXCLUSIVE) {
            return LockType::EXCLUSIVE;
        }
    }

    return LockType::SHARED;
}

bool LockManager::isCompatible(LockType existing, LockType requested) const {
    return (existing == LockType::SHARED && requested == LockType::SHARED);
}