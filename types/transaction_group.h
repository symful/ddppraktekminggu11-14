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


struct TransactionGroup* newTransactionGroup(enum TransactionCategory category, long long maximumCost);
void addTransactionToGroup(struct TransactionGroup* group, struct Transaction* transaction);
void removeTransactionFromGroup(struct TransactionGroup* group, int index);
void updateGroupCalculations(struct TransactionGroup* group);
void freeTransactionGroup(struct TransactionGroup* group);
int validateTransactionForGroup(struct TransactionGroup* group, struct Transaction* transaction);
long long getGroupBudgetUsagePercentage(struct TransactionGroup* group);
const char* getGroupBudgetStatus(struct TransactionGroup* group);

#endif
