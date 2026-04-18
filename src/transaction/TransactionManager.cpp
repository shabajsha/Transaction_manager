#include "../../include/transaction/TransactionManager.h"
#include  <sstream>
#include <cctype>

TransactionManager::TransactionManager() : next_transaction_id_(1) {
}

TransactionManager::~TransactionManager() {
}

std::string TransactionManager::trim(const std::string& value) {
    int l = 0, r = value.length() - 1;

    while (l <= r && std::isspace(static_cast<unsigned char>(value[l]))) l++;
    while (r >= l && std::isspace(static_cast<unsigned char>(value[r]))) r--;

    return value.substr(l, r - l + 1);
}

bool TransactionManager::parseTransactionLine(const std::string& line,std::shared_ptr<Transaction>& out, std::string& err){
    // Expected format: "T<transaction_id>: <operation1>, <operation2>, ..."

    int colon_pos = line.find(':');
    if(colon_pos == std::string::npos) {
        err = "Invalid transaction line format: missing ':' delimiter";
        return false;
    }
    std::string trasPart = line.substr(0, colon_pos);
    std::string opsPart = line.substr(colon_pos + 1);

    if(trasPart.empty() || trasPart[0] != 'T') {
        err = "Invalid transaction line format: missing 'T' prefix";
        return false;
    }
    if (trasPart.size() < 2) {
        err = "Missing transaction ID";
        return false;
    }
    uint32_t transaction_id;
    try{
        transaction_id = std::stoi(trasPart.substr(1));
    }
    catch(const std::exception& e){
        err = "Invalid transaction ID format: " + std::string(e.what());
        return false;
    }

    if(transaction_id == 0) {
        err = "Invalid transaction ID: must be a positive integer";
        return false;
    }

    std::string op;
    std::istringstream opsStream(opsPart);

    out = std::make_shared<Transaction>(transaction_id);

    while(std::getline(opsStream, op, ',')){
        op = trim(op);
        if(op.empty()) {
            err = "Invalid operation format: empty operation";
            return false;
        }
        if(op[0] != 'R' && op[0] != 'W') {
            err = "Invalid operation format: operation must start with 'R' or 'W'";
            return false;
        }
        if(op.length() < 4 || op[1] != '(' || op.back() != ')') {
            err = "Invalid operation format: expected format 'R(key)' or 'W(key)'";
            return false;
        }
        Operation operation;
        operation.type = (op[0] == 'R') ? OperationType::READ : OperationType::WRITE;
        operation.key = op.substr(2, op.length()-3);
        if(operation.key.empty()) {
            err = "Invalid operation format: missing key";
            return false;
        }
        out->addOperation(operation.type, operation.key);
    }
    
    return true;
}

std::shared_ptr<Transaction> TransactionManager::beginTransaction() {
    auto txn = std::make_shared<Transaction>(next_transaction_id_++);
    active_transactions_[txn->getTransactionId()] = txn;
    return txn;
}

void TransactionManager::commitTransaction(uint32_t transaction_id) {
    auto it = active_transactions_.find(transaction_id);
    if (it != active_transactions_.end()) {
        it->second->setState(TransactionState::COMMITTED);
        // TODO: Implement commit logic
        active_transactions_.erase(it);
    }
}

void TransactionManager::abortTransaction(uint32_t transaction_id) {
    auto it = active_transactions_.find(transaction_id);
    if (it != active_transactions_.end()) {
        it->second->setState(TransactionState::ABORTED);
        // TODO: Implement abort logic
        active_transactions_.erase(it);
    }
}

std::shared_ptr<Transaction> TransactionManager::getTransaction(uint32_t transaction_id) const {
    auto it = active_transactions_.find(transaction_id);
    if (it != active_transactions_.end()) {
        return it->second;
    }
    return nullptr;
}
