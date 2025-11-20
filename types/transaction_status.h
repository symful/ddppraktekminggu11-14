#ifndef TRANSACTION_STATUS_ENUM
#define TRANSACTION_STATUS_ENUM

enum TransactionStatus {
    TRANSACTION_BUDGET_SAFE,
    TRANSACTION_BUDGET_WARNING,
    TRANSACTION_BUDGET_EXCEEDED,
};

const char *transactionStatusToString(enum TransactionStatus status);

#endif
