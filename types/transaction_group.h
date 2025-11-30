#include "./transaction.h"
#include "./transaction_category.h"
#include "./transaction_status.h"

#ifndef TRANSACTION_GROUP_STRUCT_DEFINED
#define TRANSACTION_GROUP_STRUCT_DEFINED

struct TransactionGroup {
    char category[50];
    long long budget;
    long long totalAmount;
    int transactionCount;
    struct Transaction** transactions;
};

#endif
