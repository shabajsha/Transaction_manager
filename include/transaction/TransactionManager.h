#ifndef TRANSACTION_MANAGER_H
#define TRANSACTION_MANAGER_H

#include <map>
#include <memory>
#include <cstdint>
#include <string>
#include "Transaction.h"


class TransactionManager {
public:
    TransactionManager();
    ~TransactionManager();
    
    std::shared_ptr<Transaction> beginTransaction();
    void commitTransaction(uint32_t transaction_id);
    void abortTransaction(uint32_t transaction_id);
    
    std::shared_ptr<Transaction> getTransaction(uint32_t transaction_id) const;
    bool parseTransactionLine(const std::string& line,std::shared_ptr<Transaction>& out, std::string& err);
    
private:
    std::string trim(const std::string& str);
    static bool parseOperationToken(const std::string& token, Operation& op, std::string& err);

    std::map<uint32_t, std::shared_ptr<Transaction>> active_transactions_;
    uint32_t next_transaction_id_;
};

#endif // TRANSACTION_MANAGER_H
