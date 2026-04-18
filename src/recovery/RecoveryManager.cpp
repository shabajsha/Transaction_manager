#include "../../include/recovery/RecoveryManager.h"
#include <iostream>

// Constructor
RecoveryManager::RecoveryManager(std::shared_ptr<LogManager> log_manager)
    : log_manager_(log_manager), recovery_state_(RecoveryState::ANALYSIS) {}

RecoveryManager::~RecoveryManager() = default;

//  MAIN RECOVERY FUNCTION
void RecoveryManager::performRecovery() {

    std::cout << "\n===== STARTING RECOVERY =====\n";

    recovery_state_ = RecoveryState::ANALYSIS;
    analysis();

    recovery_state_ = RecoveryState::REDO;
    redo();

    recovery_state_ = RecoveryState::UNDO;
    undo();

    recovery_state_ = RecoveryState::COMPLETE;

    std::cout << "===== RECOVERY COMPLETE =====\n";
}

// ANALYSIS PHASE
void RecoveryManager::analysis() {

    std::cout << "\n[Analysis Phase]\n";

    auto logs = log_manager_->readLogs();

    for (const auto& record : logs) {

        uint32_t txn = record->getTransactionId();
        auto type = record->getLogType();

        if (type == LogRecord::LogType::BEGIN) {
            active_txns_.insert(txn);
        }

        else if (type == LogRecord::LogType::COMMIT) {
            active_txns_.erase(txn);
            committed_txns_.insert(txn);
        }

        else if (type == LogRecord::LogType::ABORT) {
            active_txns_.erase(txn);
        }
    }

    // Debug print
    std::cout << "Committed Transactions: ";
    for (auto t : committed_txns_) std::cout << t << " ";
    std::cout << std::endl;

    std::cout << "Active (Uncommitted) Transactions: ";
    for (auto t : active_txns_) std::cout << t << " ";
    std::cout << std::endl;
}

// REDO PHASE
void RecoveryManager::redo() {

    std::cout << "\n[Redo Phase]\n";

    auto logs = log_manager_->readLogs();

    for (const auto& record : logs) {

        uint32_t txn = record->getTransactionId();

        // Only redo committed transactions
        if (committed_txns_.count(txn)) {

            if (record->getLogType() == LogRecord::LogType::UPDATE) {

                std::cout << "REDO Txn " << txn
                          << " : " << record->getOldValue()
                          << " -> " << record->getNewValue()
                          << std::endl;

                // In real DB → apply new_value to storage
            }
        }
    }
}

// 🔥 UNDO PHASE
void RecoveryManager::undo() {

    std::cout << "\n[Undo Phase]\n";

    auto logs = log_manager_->readLogs();

    // Traverse logs in reverse
    for (auto it = logs.rbegin(); it != logs.rend(); ++it) {

        auto record = *it;
        uint32_t txn = record->getTransactionId();

        // Only undo active (uncommitted) transactions
        if (active_txns_.count(txn)) {

            if (record->getLogType() == LogRecord::LogType::UPDATE) {

                std::cout << "UNDO Txn " << txn
                          << " : restore " << record->getOldValue()
                          << std::endl;

                // In real DB → restore old_value
            }
        }
    }
}