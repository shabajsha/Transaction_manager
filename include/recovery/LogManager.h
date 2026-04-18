#ifndef LOG_MANAGER_H
#define LOG_MANAGER_H

#include <fstream>
#include <vector>
#include <memory>
#include <string>
#include <iostream>
#include "LogRecord.h"

class LogManager {
public:
    // Default log file path added
    LogManager(const std::string& log_file_path = "log.txt");
    ~LogManager();
    
    // Low-level API
    void writeLog(const std::shared_ptr<LogRecord>& record);
    std::vector<std::shared_ptr<LogRecord>> readLogs();
    void flushLogs();
    void clearLogs();

    // HIGH-LEVEL WAL API
    void logBegin(uint32_t txn_id);
    void logUpdate(uint32_t txn_id, const std::string& old_val, const std::string& new_val);
    void logCommit(uint32_t txn_id);
    void logAbort(uint32_t txn_id);

    // Debug / testing
    void printLogs() const;

private:
    std::ofstream log_file_;

    // WAL buffer
    std::vector<std::shared_ptr<LogRecord>> log_buffer_;
    
    // Helper
    void appendToBuffer(const std::shared_ptr<LogRecord>& record);
};

#endif // LOG_MANAGER_H