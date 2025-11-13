#ifndef TRANSACTION_TYPE_ENUM
#define TRANSACTION_TYPE_ENUM

enum TransactionType {
    TT_INCOME,
    TT_EXPENSE,
};

const char *transactionTypeToString(enum TransactionType type);

#endif
