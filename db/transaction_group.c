#include "../types/transaction_group.h"
#include "./transaction_group.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef TRANSACTION_GROUP_C
#define TRANSACTION_GROUP_C

struct TransactionGroup *newTransactionGroup(enum TransactionCategory category,
                                             long long maximumCost) {
  struct TransactionGroup *group =
      (struct TransactionGroup *)malloc(sizeof(struct TransactionGroup));
  if (group == NULL) {
    perror("Error allocating memory for transaction group");
    return NULL;
  }

  group->category = category;
  group->maximumCost = maximumCost;
  group->transactionsAmount = 0;
  group->transactions = NULL;
  group->totalRealCost = 0;
  group->remainingCost = maximumCost;

  return group;
}

void addTransactionToGroup(struct TransactionGroup *group,
                           struct Transaction *transaction) {
  if (group == NULL || transaction == NULL) {
    return;
  }

  struct Transaction *temp = (struct Transaction *)realloc(
      group->transactions,
      (group->transactionsAmount + 1) * sizeof(struct Transaction));

  if (temp == NULL) {
    perror("Error reallocating memory for transactions in group");
    return;
  }

  group->transactions = temp;
  group->transactions[group->transactionsAmount] = *transaction;
  group->transactionsAmount++;

  updateGroupCalculations(group);
}

void removeTransactionFromGroup(struct TransactionGroup *group, int index) {
  if (group == NULL || index < 0 || index >= group->transactionsAmount) {
    return;
  }

  for (int i = index; i < group->transactionsAmount - 1; i++) {
    group->transactions[i] = group->transactions[i + 1];
  }

  group->transactionsAmount--;

  if (group->transactionsAmount > 0) {
    struct Transaction *temp = (struct Transaction *)realloc(
        group->transactions,
        group->transactionsAmount * sizeof(struct Transaction));
    if (temp != NULL) {
      group->transactions = temp;
    }
  } else {
    free(group->transactions);
    group->transactions = NULL;
  }

  updateGroupCalculations(group);
}

void updateGroupCalculations(struct TransactionGroup *group) {
  if (group == NULL) {
    return;
  }

  group->totalRealCost = 0;

  for (int i = 0; i < group->transactionsAmount; i++) {
    if (group->transactions[i].type == TT_EXPENSE) {
      group->totalRealCost += group->transactions[i].realCost;
    }
  }

  group->remainingCost = group->maximumCost - group->totalRealCost;
}

void freeTransactionGroup(struct TransactionGroup *group) {
  if (group == NULL) {
    return;
  }

  if (group->transactions != NULL) {
    free(group->transactions);
    group->transactions = NULL;
  }

  free(group);
}

int validateTransactionForGroup(struct TransactionGroup *group,
                                struct Transaction *transaction) {
  if (group == NULL || transaction == NULL) {
    return 0;
  }

  if (transaction->type != TT_EXPENSE) {
    return 1;
  }

  if (group->totalRealCost + transaction->realCost > group->maximumCost) {
    return 0;
  }

  return 1;
}

long long getGroupBudgetUsagePercentage(struct TransactionGroup *group) {
  if (group == NULL || group->maximumCost == 0) {
    return 0;
  }

  return (group->totalRealCost * 100) / group->maximumCost;
}

const char *getGroupBudgetStatus(struct TransactionGroup *group) {
  if (group == NULL) {
    return "Unknown";
  }

  if (group->remainingCost >= 0) {
    long long percentage = getGroupBudgetUsagePercentage(group);
    if (percentage >= 90) {
      return "Hampir Habis";
    } else if (percentage >= 75) {
      return "Waspada";
    } else {
      return "Aman";
    }
  } else {
    return "Melebihi Budget";
  }
}

#endif
