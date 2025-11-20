#include "./transaction.h"
#include "./transaction_category.h"
#include "./transaction_status.h"

#ifndef TRANSACTION_GROUP_STRUCT_DEFINED
#define TRANSACTION_GROUP_STRUCT_DEFINED

struct TransactionGroup {
    enum TransactionCategory category;
    long long budget;                    // Changed from maximumCost for clarity
    long long totalAmount;              // Changed from totalRealCost for consistency
    int transactionCount;               // Changed from transactionsAmount for consistency
    struct Transaction** transactions;   // Changed to array of pointers for better memory management
};

#endif