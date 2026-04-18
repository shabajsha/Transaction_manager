#ifndef LOCK_MANAGER_H
#define LOCK_MANAGER_H

#include <map>
#include <set>
#include <memory>
#include <vector>
#include <queue>
#include <cstdint>
#include "../utils/enums.h"
#include "Lock.h"
#include "WaitForGraph.h"

class LockManager {
public:
    LockManager();
    ~LockManager();
    
    // Request a lock on a resource
    bool requestLock(uint32_t transaction_id, uint32_t resource_id, LockType lock_type);
    
    // Release a lock held by a transaction
    bool releaseLock(uint32_t transaction_id, uint32_t resource_id);
    
    // Check if a transaction already holds a lock on a resource
    bool hasLock(uint32_t transaction_id, uint32_t resource_id) const;
    
    // Get the lock type currently held on a resource (simplified)
    LockType getLockType(uint32_t resource_id) const;
    
private:
    std::map<uint32_t, std::set<std::shared_ptr<Lock>>> resource_locks_;
    void abortTransaction(uint32_t txn_id);
    // Wait-for graph for deadlock detection
    WaitForGraph wait_for_graph_;
    // resource_id -> set of locks held on that resource
    std::map<uint32_t, std::vector<std::shared_ptr<Lock>>> resource_locks_;
    // resource_id -> queue of (transaction_id, requested_lock_type)
    std::map<uint32_t, std::queue<std::pair<uint32_t, LockType>>> waiting_queue_;
    // Check compatibility between existing and requested lock
    bool isCompatible(LockType existing, LockType requested) const;
};

#endif // LOCK_MANAGER_H