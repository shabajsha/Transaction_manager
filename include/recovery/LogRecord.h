#ifndef LOG_RECORD_H
#define LOG_RECORD_H

#include <cstdint>
#include <string>
#include <ctime>

class LogRecord {
public:
    enum class LogType {
        BEGIN,
        INSERT,
        UPDATE,
        DELETE,
        COMMIT,
        ABORT,
        CHECKPOINT
    };

    // Constructor for non-write logs
    LogRecord(uint32_t transaction_id, LogType type);

    // Constructor for write logs (INSERT/UPDATE/DELETE)
    LogRecord(uint32_t transaction_id, LogType type,
              const std::string& old_value,
              const std::string& new_value);

    ~LogRecord();

    uint32_t getTransactionId() const;
    LogType getLogType() const;
    time_t getTimestamp() const;

    //   for WAL
    std::string getOldValue() const;
    std::string getNewValue() const;

    // Debug print
    void print() const;

private:
    uint32_t transaction_id_;
    LogType type_;
    time_t timestamp_;

    // WRITE operations
    std::string old_value_;
    std::string new_value_;
};

#endif // LOG_RECORD_H