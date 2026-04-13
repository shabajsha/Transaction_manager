#include "../../include/concurrency/LockManager.h"

LockManager::LockManager() {}

LockManager::~LockManager() = default;

bool LockManager::requestLock(uint32_t transaction_id, uint32_t resource_id, LockType lock_type) {
    // Step 0: If already holds lock → do nothing
    if (hasLock(transaction_id, resource_id)) {
        return true;
    }
    // Step 1: Check existing locks
    auto it = resource_locks_.find(resource_id);
    if (it != resource_locks_.end()) {
        for (const auto& existing_lock : it->second) {
            if (existing_lock->getTransactionId() == transaction_id) {
                continue;
            }
            if (!isCompatible(existing_lock->getLockType(), lock_type)) {
                //block instead of deny
                waiting_queue_[resource_id].push({transaction_id, lock_type});
                return false;
            }
        }
    }
    // Step 2: Grant lock
    auto new_lock = std::make_shared<Lock>(transaction_id, lock_type);
    resource_locks_[resource_id].push_back(new_lock);
    return true;
}

bool LockManager::releaseLock(uint32_t transaction_id, uint32_t resource_id) {
    auto it = resource_locks_.find(resource_id);
    if (it != resource_locks_.end()) {
        auto& locks = it->second;
        for (auto lock_it = locks.begin(); lock_it != locks.end(); ) {
            if ((*lock_it)->getTransactionId() == transaction_id) {
                lock_it = locks.erase(lock_it);
            } else {
                ++lock_it;
            }
        }
        // process waiting queue
        auto& queue = waiting_queue_[resource_id];
        while (!queue.empty()) {
            auto [txn_id, lock_type] = queue.front();
            if (requestLock(txn_id, resource_id, lock_type)) {
                queue.pop();
            } else {
                break;
            }
        }
        return true;
    }
    return false;
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
        return LockType::SHARED; // placeholder (we improve later)
    }

    // If ANY exclusive lock exists → return EXCLUSIVE
    for (const auto& lock : it->second) {
        if (lock->getLockType() == LockType::EXCLUSIVE) {
            return LockType::EXCLUSIVE;
        }
    }

    return LockType::SHARED;
}

bool LockManager::isCompatible(LockType existing, LockType requested) const {
    // Only SHARED + SHARED is allowed
    if (existing == LockType::SHARED && requested == LockType::SHARED) {
        return true;
    }
    return false;
}