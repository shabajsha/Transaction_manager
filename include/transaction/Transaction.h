#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <cstdint>
#include <chrono>
#include "../utils/enums.h"
#include <string>
#include <vector>

// using namespace std;


struct Operation{
    OperationType type;
    std::string key; // the key being accessed
};

class Transaction {
public:
    Transaction(uint32_t transaction_id);
    ~Transaction() = default;
    std::vector<Operation> operations; // List of operations in the transaction

    uint32_t getTransactionId() const;
    TransactionState getState() const;
    void setState(TransactionState state);
    
    std::chrono::system_clock::time_point getStartTime() const;
    
    // TODO: Implement transaction-specific methods
    void addOperation(OperationType type, const std::string& key) {
        operations.push_back({type, key});
    }

    
    
private:
    uint32_t transaction_id_;
    TransactionState state_;
    std::chrono::system_clock::time_point start_time_;
};

#endif // TRANSACTION_H
