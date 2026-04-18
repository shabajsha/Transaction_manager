#ifndef LOCK_MANAGER_H
#define LOCK_MANAGER_H

#include <map>
#include <set>
#include <memory>
#include <cstdint>
#include "../utils/enums.h"
#include "Lock.h"
#include "WaitForGraph.h"
#include "../recovery/LogManager.h"

class LockManager {
public:
    LockManager();
    ~LockManager();

    bool requestLock(uint32_t transaction_id, uint32_t resource_id, LockType lock_type);
    bool releaseLock(uint32_t transaction_id, uint32_t resource_id);

    bool hasLock(uint32_t transaction_id, uint32_t resource_id) const;
    LockType getLockType(uint32_t resource_id) const;

private:
    std::map<uint32_t, std::set<std::shared_ptr<Lock>>> resource_locks_;

    WaitForGraph wait_for_graph_;

    LogManager log_manager_;

    void abortTransaction(uint32_t txn_id);

    bool isCompatible(LockType existing, LockType requested) const;
};

#endif