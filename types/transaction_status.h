#ifndef TRANSACTION_STATUS_ENUM
#define TRANSACTION_STATUS_ENUM

enum TransactionStatus {
    TS_SAFE,
    TS_OVER,
};

const char *transactionStatusToString(enum TransactionStatus status);

#endif
