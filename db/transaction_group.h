#include "../types/transaction_group.h"
#include "../types/transaction.h"
#include "../types/transaction_category.h"

#ifndef DB_TRANSACTION_GROUP_H
#define DB_TRANSACTION_GROUP_H


struct TransactionGroup* newTransactionGroup(enum TransactionCategory category, long long maximumCost);
void addTransactionToGroup(struct TransactionGroup* group, struct Transaction* transaction);
void removeTransactionFromGroup(struct TransactionGroup* group, int index);
void updateGroupCalculations(struct TransactionGroup* group);
void freeTransactionGroup(struct TransactionGroup* group);
int validateTransactionForGroup(struct TransactionGroup* group, struct Transaction* transaction);
long long getGroupBudgetUsagePercentage(struct TransactionGroup* group);
const char* getGroupBudgetStatus(struct TransactionGroup* group);

#endif
