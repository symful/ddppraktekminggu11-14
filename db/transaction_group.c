#include "../types/transaction_group.h"
#include "./transaction_group.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef TRANSACTION_GROUP_C
#define TRANSACTION_GROUP_C

struct TransactionGroup *newTransactionGroup(enum TransactionCategory category,
                                             long long budget) {
  struct TransactionGroup *group =
      (struct TransactionGroup *)malloc(sizeof(struct TransactionGroup));
  if (group == NULL) {
    perror("Error allocating memory for transaction group");
    return NULL;
  }

  group->category = category;
  group->budget = budget;
  group->transactionCount = 0;
  group->transactions = NULL;
  group->totalAmount = 0;

  return group;
}

void addTransactionToGroup(struct TransactionGroup *group,
                           struct Transaction *transaction) {
  if (group == NULL || transaction == NULL) {
    return;
  }

  // Reallocate memory for the new transaction pointer
  struct Transaction **newTransactions = (struct Transaction **)realloc(
      group->transactions,
      (group->transactionCount + 1) * sizeof(struct Transaction *));

  if (newTransactions == NULL) {
    perror("Error reallocating memory for transactions in group");
    return;
  }

  group->transactions = newTransactions;
  group->transactions[group->transactionCount] = transaction;
  group->transactionCount++;

  updateGroupCalculations(group);
}

void removeTransactionFromGroup(struct TransactionGroup *group, int index) {
  if (group == NULL || index < 0 || index >= group->transactionCount) {
    return;
  }

  // Free the transaction at the specified index
  if (group->transactions[index] != NULL) {
    free(group->transactions[index]);
  }

  // Shift remaining transactions
  for (int i = index; i < group->transactionCount - 1; i++) {
    group->transactions[i] = group->transactions[i + 1];
  }

  group->transactionCount--;

  if (group->transactionCount > 0) {
    // Reallocate to smaller size
    struct Transaction **newTransactions = (struct Transaction **)realloc(
        group->transactions,
        group->transactionCount * sizeof(struct Transaction *));
    if (newTransactions != NULL) {
      group->transactions = newTransactions;
    }
    // If realloc fails, we still have the valid smaller array
  } else {
    // No more transactions, free the array
    free(group->transactions);
    group->transactions = NULL;
  }

  updateGroupCalculations(group);
}

void updateGroupCalculations(struct TransactionGroup *group) {
  if (group == NULL) {
    return;
  }

  group->totalAmount = 0;

  for (int i = 0; i < group->transactionCount; i++) {
    if (group->transactions[i] != NULL) {
      // Sum all transaction amounts regardless of type for group totals
      // Individual income/expense tracking is handled at report level
      group->totalAmount += group->transactions[i]->amount;
    }
  }
}

void freeTransactionGroup(struct TransactionGroup *group) {
  if (group == NULL) {
    return;
  }

  if (group->transactions != NULL) {
    // Free each individual transaction
    for (int i = 0; i < group->transactionCount; i++) {
      if (group->transactions[i] != NULL) {
        free(group->transactions[i]);
      }
    }
    // Free the array of pointers
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

  // Income transactions are always valid
  if (transaction->type == TRANSACTION_INCOME) {
    return 1;
  }

  // For expense transactions, check if it would exceed budget
  if (transaction->type == TRANSACTION_EXPENSE) {
    if (group->totalAmount + transaction->amount > group->budget) {
      return 0; // Would exceed budget
    }
  }

  return 1;
}

long long getGroupBudgetUsagePercentage(const struct TransactionGroup *group) {
  if (group == NULL || group->budget == 0) {
    return 0;
  }

  return (group->totalAmount * 100) / group->budget;
}

const char *getGroupBudgetStatus(const struct TransactionGroup *group) {
  if (group == NULL) {
    return "Unknown";
  }

  if (group->budget <= 0) {
    return "No Budget";
  }

  long long remaining = group->budget - group->totalAmount;

  if (remaining < 0) {
    return "Over Budget";
  } else if (remaining == 0) {
    return "Budget Exhausted";
  } else {
    long long percentage = getGroupBudgetUsagePercentage(group);
    if (percentage >= 90) {
      return "Critical";
    } else if (percentage >= 75) {
      return "Warning";
    } else if (percentage >= 50) {
      return "Caution";
    } else {
      return "Safe";
    }
  }
}

long long getGroupRemainingBudget(const struct TransactionGroup *group) {
  if (group == NULL) {
    return 0;
  }

  return group->budget - group->totalAmount;
}

int getTransactionCountByType(const struct TransactionGroup *group,
                              enum TransactionType type) {
  if (group == NULL) {
    return 0;
  }

  int count = 0;
  for (int i = 0; i < group->transactionCount; i++) {
    if (group->transactions[i] != NULL &&
        group->transactions[i]->type == type) {
      count++;
    }
  }

  return count;
}

struct Transaction *findTransactionById(const struct TransactionGroup *group,
                                        int transactionId) {
  if (group == NULL) {
    return NULL;
  }

  for (int i = 0; i < group->transactionCount; i++) {
    if (group->transactions[i] != NULL &&
        group->transactions[i]->id == transactionId) {
      return group->transactions[i];
    }
  }

  return NULL;
}

void sortTransactionsByDate(struct TransactionGroup *group, int ascending) {
  if (group == NULL || group->transactionCount <= 1) {
    return;
  }

  // Simple bubble sort by date
  for (int i = 0; i < group->transactionCount - 1; i++) {
    for (int j = 0; j < group->transactionCount - i - 1; j++) {
      if (group->transactions[j] != NULL &&
          group->transactions[j + 1] != NULL) {
        int shouldSwap = ascending ? (group->transactions[j]->date >
                                      group->transactions[j + 1]->date)
                                   : (group->transactions[j]->date <
                                      group->transactions[j + 1]->date);

        if (shouldSwap) {
          struct Transaction *temp = group->transactions[j];
          group->transactions[j] = group->transactions[j + 1];
          group->transactions[j + 1] = temp;
        }
      }
    }
  }
}

void sortTransactionsByAmount(struct TransactionGroup *group, int ascending) {
  if (group == NULL || group->transactionCount <= 1) {
    return;
  }

  // Simple bubble sort by amount
  for (int i = 0; i < group->transactionCount - 1; i++) {
    for (int j = 0; j < group->transactionCount - i - 1; j++) {
      if (group->transactions[j] != NULL &&
          group->transactions[j + 1] != NULL) {
        int shouldSwap = ascending ? (group->transactions[j]->amount >
                                      group->transactions[j + 1]->amount)
                                   : (group->transactions[j]->amount <
                                      group->transactions[j + 1]->amount);

        if (shouldSwap) {
          struct Transaction *temp = group->transactions[j];
          group->transactions[j] = group->transactions[j + 1];
          group->transactions[j + 1] = temp;
        }
      }
    }
  }
}

#endif
