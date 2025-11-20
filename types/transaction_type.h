#ifndef TRANSACTION_TYPE_ENUM
#define TRANSACTION_TYPE_ENUM

enum TransactionType {
    TRANSACTION_INCOME,
    TRANSACTION_EXPENSE,
};

const char *transactionTypeToString(enum TransactionType type);

#endif