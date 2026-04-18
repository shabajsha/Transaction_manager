#include "../../include/recovery/LogRecord.h"
#include <iostream>

// Constructor for BEGIN, COMMIT, ABORT.
LogRecord::LogRecord(uint32_t transaction_id, LogType type)
    : transaction_id_(transaction_id),
      type_(type),
      timestamp_(time(nullptr)),
      old_value_(""),
      new_value_("") {
}

// Constructor for WRITE operations (INSERT/UPDATE/DELETE)
LogRecord::LogRecord(uint32_t transaction_id, LogType type,
                     const std::string& old_value,
                     const std::string& new_value)
    : transaction_id_(transaction_id),
      type_(type),
      timestamp_(time(nullptr)),
      old_value_(old_value),
      new_value_(new_value) {
}

LogRecord::~LogRecord() = default;

// Getters
uint32_t LogRecord::getTransactionId() const {
    return transaction_id_;
}

LogRecord::LogType LogRecord::getLogType() const {
    return type_;
}

time_t LogRecord::getTimestamp() const {
    return timestamp_;
}

//  for WAL
std::string LogRecord::getOldValue() const {
    return old_value_;
}

std::string LogRecord::getNewValue() const {
    return new_value_;
}

//  DEBUG PRINT
void LogRecord::print() const {
    std::cout << "Txn " << transaction_id_ << " | ";

    switch (type_) {
        case LogType::BEGIN:
            std::cout << "BEGIN";
            break;
        case LogType::INSERT:
            std::cout << "INSERT | " << new_value_;
            break;
        case LogType::UPDATE:
            std::cout << "UPDATE | " << old_value_ << " -> " << new_value_;
            break;
        case LogType::DELETE:
            std::cout << "DELETE | " << old_value_;
            break;
        case LogType::COMMIT:
            std::cout << "COMMIT";
            break;
        case LogType::ABORT:
            std::cout << "ABORT";
            break;
        case LogType::CHECKPOINT:
            std::cout << "CHECKPOINT";
            break;
    }

    std::cout << std::endl;
}