#include <iostream>
#include <memory>
#include <vector>

#include "transaction/TransactionManager.h"
#include "concurrency/LockManager.h"
#include "recovery/LogManager.h"
#include "storage/DataStore.h"

void test_basic(TransactionManager& tm, LockManager& lm) {
    std::cout << "\n=== TEST 1: BASIC TRANSACTION ===\n";

    auto txn = tm.beginTransaction();

    bool granted = lm.requestLock(txn->getTransactionId(), 1, LockType::EXCLUSIVE);

    if (granted)
        std::cout << "PASS: Lock granted\n";
    else
        std::cout << "FAIL: Lock not granted\n";

    tm.commitTransaction(txn->getTransactionId());
    lm.releaseLock(txn->getTransactionId(), 1);
}

void test_lock_conflict(LockManager& lm) {
    std::cout << "\n=== TEST 2: LOCK CONFLICT ===\n";

    lm.requestLock(1, 100, LockType::EXCLUSIVE);

    bool granted = lm.requestLock(2, 100, LockType::EXCLUSIVE);

    if (!granted)
        std::cout << "PASS: Conflict detected (txn2 blocked)\n";
    else
        std::cout << "FAIL: Conflict NOT detected\n";

    lm.releaseLock(1, 100);
}

void test_shared_locks(LockManager& lm) {
    std::cout << "\n=== TEST 3: SHARED LOCKS ===\n";

    bool t1 = lm.requestLock(1, 200, LockType::SHARED);
    bool t2 = lm.requestLock(2, 200, LockType::SHARED);

    if (t1 && t2)
        std::cout << "PASS: Shared locks allowed\n";
    else
        std::cout << "FAIL: Shared locks broken\n";

    lm.releaseLock(1, 200);
    lm.releaseLock(2, 200);
}

void test_deadlock(LockManager& lm) {
    std::cout << "\n=== TEST 4: DEADLOCK ===\n";

    lm.requestLock(1, 300, LockType::EXCLUSIVE);
    lm.requestLock(2, 400, LockType::EXCLUSIVE);

    lm.requestLock(1, 400, LockType::EXCLUSIVE); // waits
    bool result = lm.requestLock(2, 300, LockType::EXCLUSIVE); // DEADLOCK

    if (!result)
        std::cout << "PASS: Deadlock handled\n";
    else
        std::cout << "FAIL: Deadlock NOT detected\n";

    lm.releaseLock(1, 300);
    lm.releaseLock(2, 400);
}

void test_multiple_resources(LockManager& lm) {
    std::cout << "\n=== TEST 5: MULTIPLE RESOURCES ===\n";

    lm.requestLock(1, 10, LockType::EXCLUSIVE);
    lm.requestLock(1, 20, LockType::EXCLUSIVE);

    bool t2 = lm.requestLock(2, 10, LockType::EXCLUSIVE);

    if (!t2)
        std::cout << "PASS: Resource isolation works\n";
    else
        std::cout << "FAIL: Isolation broken\n";

    lm.releaseLock(1, 10);
    lm.releaseLock(1, 20);
}

void test_logging(LogManager& log_mgr) {
    std::cout << "\n=== TEST 6: LOGGING ===\n";

    auto log1 = std::make_shared<LogRecord>(1, LogRecord::LogType::BEGIN);
    auto log2 = std::make_shared<LogRecord>(1, LogRecord::LogType::COMMIT);

    log_mgr.writeLog(log1);
    log_mgr.writeLog(log2);
    log_mgr.flushLogs();

    std::cout << "Check transaction.log file manually\n";
}

void test_transaction_parsing(TransactionManager& tm) {
    std::cout << "\n=== TEST 7: TRANSACTION PARSING ===\n";

    std::string line = "T5: R(x), W(y), R(z)";
    std::shared_ptr<Transaction> txn;
    std::string err;

    if (tm.parseTransactionLine(line, txn, err))
        std::cout << "PASS: Parsing success\n";
    else
        std::cout << "FAIL: " << err << "\n";
}

void test_invalid_parsing(TransactionManager& tm) {
    std::cout << "\n=== TEST 8: INVALID INPUT ===\n";

    std::string line = "INVALID INPUT";
    std::shared_ptr<Transaction> txn;
    std::string err;

    if (!tm.parseTransactionLine(line, txn, err))
        std::cout << "PASS: Invalid input detected\n";
    else
        std::cout << "FAIL: Invalid input accepted\n";
}

int main() {
    std::cout << "=== FULL SYSTEM TEST ===\n";

    TransactionManager tm;
    LockManager lm;
    LogManager log_mgr("transaction.log");
    DataStore ds;

    test_basic(tm, lm);
    test_lock_conflict(lm);
    test_shared_locks(lm);
    test_deadlock(lm);
    test_multiple_resources(lm);
    test_logging(log_mgr);
    test_transaction_parsing(tm);
    test_invalid_parsing(tm);

    std::cout << "\n=== ALL TESTS COMPLETED ===\n";

    return 0;
}