#include "../types/transaction_group.h"
#include "../types/transaction.h"
#include "../types/transaction_category.h"
#include "../types/transaction_type.h"

#ifndef DB_TRANSACTION_GROUP_H
#define DB_TRANSACTION_GROUP_H


struct TransactionGroup* newTransactionGroup(const char* category, long long budget);
void addTransactionToGroup(struct TransactionGroup* group, struct Transaction* transaction);
void removeTransactionFromGroup(struct TransactionGroup* group, int index);
void updateGroupCalculations(struct TransactionGroup* group);
void freeTransactionGroup(struct TransactionGroup* group);


int validateTransactionForGroup(struct TransactionGroup* group, struct Transaction* transaction);
long long getGroupBudgetUsagePercentage(const struct TransactionGroup* group);
const char* getGroupBudgetStatus(const struct TransactionGroup* group);
long long getGroupRemainingBudget(const struct TransactionGroup* group);


int getTransactionCountByType(const struct TransactionGroup* group, enum TransactionType type);
struct Transaction* findTransactionById(const struct TransactionGroup* group, int transactionId);


void sortTransactionsByDate(struct TransactionGroup* group, int ascending);
void sortTransactionsByAmount(struct TransactionGroup* group, int ascending);

#endif
