#ifndef ENUMS_H
#define ENUMS_H

enum class OperationType{
    READ,
    WRITE
};

enum class TransactionState {
    ACTIVE,
    COMMITTED,
    ABORTED,
    WAITING
};

enum class LockType {
    SHARED,
    EXCLUSIVE,
    INTENTION_SHARED,
    INTENTION_EXCLUSIVE
};

enum class RecoveryState {
    ANALYSIS,
    REDO,
    UNDO,
    COMPLETE
};

#endif // ENUMS_H
