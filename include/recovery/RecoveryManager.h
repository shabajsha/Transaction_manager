#ifndef RECOVERY_MANAGER_H
#define RECOVERY_MANAGER_H

#include <memory>
#include <vector>
#include <set>
#include "LogManager.h"
#include "../utils/enums.h"

class RecoveryManager {
public:
    RecoveryManager(std::shared_ptr<LogManager> log_manager);
    ~RecoveryManager();
    
    // MAIN ENTRY
    void performRecovery();

    // PHASES
    void analysis();   // identify committed & active txns
    void redo();       // reapply committed transactions
    void undo();       // rollback incomplete transactions
    
private:
    std::shared_ptr<LogManager> log_manager_;

    RecoveryState recovery_state_;

    std::set<uint32_t> committed_txns_;
    std::set<uint32_t> active_txns_;
};

#endif // RECOVERY_MANAGER_H