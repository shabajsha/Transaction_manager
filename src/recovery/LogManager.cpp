#include "../../include/recovery/LogManager.h"
#include <iostream>

// Constructor
LogManager::LogManager(const std::string& log_file_path) {
    log_file_.open(log_file_path, std::ios::app);
}

// Destructor
LogManager::~LogManager() {
    if (log_file_.is_open()) {
        flushLogs();  // ensure logs are written
        log_file_.close();
    }
}

//  INTERNAL HELPER
void LogManager::appendToBuffer(const std::shared_ptr<LogRecord>& record) {
    log_buffer_.push_back(record);
}

// LOW-LEVEL WRITE
void LogManager::writeLog(const std::shared_ptr<LogRecord>& record) {
    appendToBuffer(record);
}

// READ LOGS (for now from buffer)
std::vector<std::shared_ptr<LogRecord>> LogManager::readLogs() {
    return log_buffer_;
}

// FLUSH TO FILE (WAL step)
void LogManager::flushLogs() {
    if (!log_file_.is_open()) return;

    for (const auto& record : log_buffer_) {

        log_file_ << "Txn " << record->getTransactionId() << " | ";

        switch (record->getLogType()) {
            case LogRecord::LogType::BEGIN:
                log_file_ << "BEGIN";
                break;

            case LogRecord::LogType::INSERT:
                log_file_ << "INSERT | " << record->getNewValue();
                break;

            case LogRecord::LogType::UPDATE:
                log_file_ << "UPDATE | "
                          << record->getOldValue() << " -> "
                          << record->getNewValue();
                break;

            case LogRecord::LogType::DELETE:
                log_file_ << "DELETE | " << record->getOldValue();
                break;

            case LogRecord::LogType::COMMIT:
                log_file_ << "COMMIT";
                break;

            case LogRecord::LogType::ABORT:
                log_file_ << "ABORT";
                break;

            case LogRecord::LogType::CHECKPOINT:
                log_file_ << "CHECKPOINT";
                break;
        }

        log_file_ << std::endl;
    }

    log_file_.flush();
    log_buffer_.clear();
}

// CLEAR BUFFER
void LogManager::clearLogs() {
    log_buffer_.clear();
}

// HIGH-LEVEL WAL API

// BEGIN
void LogManager::logBegin(uint32_t txn_id) {
    auto record = std::make_shared<LogRecord>(txn_id, LogRecord::LogType::BEGIN);
    appendToBuffer(record);
}

// UPDATE (WRITE)
void LogManager::logUpdate(uint32_t txn_id,
                          const std::string& old_val,
                          const std::string& new_val) {
    auto record = std::make_shared<LogRecord>(
        txn_id,
        LogRecord::LogType::UPDATE,
        old_val,
        new_val
    );
    appendToBuffer(record);
}

// COMMIT
void LogManager::logCommit(uint32_t txn_id) {
    auto record = std::make_shared<LogRecord>(txn_id, LogRecord::LogType::COMMIT);
    appendToBuffer(record);

    // WAL rule: flush on commit
    flushLogs();
}

// ABORT
void LogManager::logAbort(uint32_t txn_id) {
    auto record = std::make_shared<LogRecord>(txn_id, LogRecord::LogType::ABORT);
    appendToBuffer(record);

    flushLogs();
}

// DEBUG PRINT
void LogManager::printLogs() const {
    for (const auto& record : log_buffer_) {
        record->print();
    }
}