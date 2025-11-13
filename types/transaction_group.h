#include "./transaction.h"
#include "./transaction_category.h"

#ifndef TRANSACTION_GROUP_STRUCT_DEFINED
#define TRANSACTION_GROUP_STRUCT_DEFINED

struct TransactionGroup {
    enum TransactionCategory category;
    long long maximumCost;
    int transactionsAmount;
    struct Transaction* transactions;
    long long totalRealCost;
    long long remainingCost;
};

#endif
