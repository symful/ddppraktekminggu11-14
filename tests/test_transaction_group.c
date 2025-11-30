#include "../db/transaction_group.h"
#include "../types/transaction.h"
#include "../types/transaction_group.h"
#include "../types/transaction_type.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct User {
  char username[50];
  char userDir[256];
  char hashFile[256];
  int isAdmin;
};

struct User *currentUser = NULL;

void test_new_transaction_group() {
  printf("\n=== Test: New Transaction Group ===\n");

  struct TransactionGroup *group = newTransactionGroup("food", 1000000);
  assert(group != NULL);
  assert(strcmp(group->category, "food") == 0);
  assert(group->budget == 1000000);
  assert(group->totalAmount == 0);
  assert(group->transactionCount == 0);
  assert(group->transactions == NULL);

  freeTransactionGroup(group);
  printf("✓ New transaction group created successfully\n");
}

void test_transaction_group_category_string() {
  printf("\n=== Test: Transaction Group Category as String ===\n");

  struct TransactionGroup *group1 =
      newTransactionGroup("transportation", 500000);
  struct TransactionGroup *group2 = newTransactionGroup("income", 2000000);
  struct TransactionGroup *group3 =
      newTransactionGroup("entertainment", 750000);

  assert(strcmp(group1->category, "transportation") == 0);
  assert(strcmp(group2->category, "income") == 0);
  assert(strcmp(group3->category, "entertainment") == 0);

  freeTransactionGroup(group1);
  freeTransactionGroup(group2);
  freeTransactionGroup(group3);
  printf("✓ String-based categories work correctly\n");
}

void test_add_transaction_to_group() {
  printf("\n=== Test: Add Transaction to Group ===\n");

  struct TransactionGroup *group = newTransactionGroup("food", 1000000);

  struct Transaction *trans1 =
      (struct Transaction *)malloc(sizeof(struct Transaction));
  trans1->id = 1;
  trans1->amount = 50000;
  trans1->type = TRANSACTION_EXPENSE;
  strncpy(trans1->category, "food", sizeof(trans1->category) - 1);
  trans1->category[sizeof(trans1->category) - 1] = '\0';
  trans1->date = time(NULL);
  strcpy(trans1->name, "Groceries");

  addTransactionToGroup(group, trans1);

  assert(group->transactionCount == 1);
  assert(group->totalAmount == 50000);
  assert(group->transactions[0] == trans1);

  struct Transaction *trans2 =
      (struct Transaction *)malloc(sizeof(struct Transaction));
  trans2->id = 2;
  trans2->amount = 30000;
  trans2->type = TRANSACTION_EXPENSE;
  strncpy(trans2->category, "food", sizeof(trans2->category) - 1);
  trans2->category[sizeof(trans2->category) - 1] = '\0';
  trans2->date = time(NULL);
  strcpy(trans2->name, "Restaurant");

  addTransactionToGroup(group, trans2);

  assert(group->transactionCount == 2);
  assert(group->totalAmount == 80000);

  freeTransactionGroup(group);
  printf("✓ Transactions added to group successfully\n");
}

void test_remove_transaction_from_group() {
  printf("\n=== Test: Remove Transaction from Group ===\n");

  struct TransactionGroup *group = newTransactionGroup("food", 1000000);

  struct Transaction *trans1 =
      (struct Transaction *)malloc(sizeof(struct Transaction));
  trans1->id = 1;
  trans1->amount = 50000;
  trans1->type = TRANSACTION_EXPENSE;
  strncpy(trans1->category, "food", sizeof(trans1->category) - 1);

  struct Transaction *trans2 =
      (struct Transaction *)malloc(sizeof(struct Transaction));
  trans2->id = 2;
  trans2->amount = 30000;
  trans2->type = TRANSACTION_EXPENSE;
  strncpy(trans2->category, "food", sizeof(trans2->category) - 1);

  addTransactionToGroup(group, trans1);
  addTransactionToGroup(group, trans2);

  assert(group->transactionCount == 2);
  assert(group->totalAmount == 80000);

  removeTransactionFromGroup(group, 0);

  assert(group->transactionCount == 1);
  assert(group->totalAmount == 30000);
  assert(group->transactions[0]->id == 2);

  freeTransactionGroup(group);
  printf("✓ Transaction removed from group successfully\n");
}

void test_update_group_calculations() {
  printf("\n=== Test: Update Group Calculations ===\n");

  struct TransactionGroup *group = newTransactionGroup("entertainment", 500000);

  struct Transaction *trans1 =
      (struct Transaction *)malloc(sizeof(struct Transaction));
  trans1->amount = 100000;
  struct Transaction *trans2 =
      (struct Transaction *)malloc(sizeof(struct Transaction));
  trans2->amount = 150000;
  struct Transaction *trans3 =
      (struct Transaction *)malloc(sizeof(struct Transaction));
  trans3->amount = 75000;

  addTransactionToGroup(group, trans1);
  addTransactionToGroup(group, trans2);
  addTransactionToGroup(group, trans3);

  assert(group->totalAmount == 325000);
  assert(group->transactionCount == 3);

  freeTransactionGroup(group);
  printf("✓ Group calculations updated correctly\n");
}

void test_budget_status() {
  printf("\n=== Test: Budget Status ===\n");

  struct TransactionGroup *group = newTransactionGroup("shopping", 1000000);

  const char *status = getGroupBudgetStatus(group);
  assert(strcmp(status, "Safe") == 0);

  struct Transaction *trans1 =
      (struct Transaction *)malloc(sizeof(struct Transaction));
  trans1->amount = 600000;
  addTransactionToGroup(group, trans1);

  status = getGroupBudgetStatus(group);
  assert(strcmp(status, "Caution") == 0);

  struct Transaction *trans2 =
      (struct Transaction *)malloc(sizeof(struct Transaction));
  trans2->amount = 300000;
  addTransactionToGroup(group, trans2);

  status = getGroupBudgetStatus(group);
  assert(strcmp(status, "Critical") == 0);

  struct Transaction *trans3 =
      (struct Transaction *)malloc(sizeof(struct Transaction));
  trans3->amount = 200000;
  addTransactionToGroup(group, trans3);

  status = getGroupBudgetStatus(group);
  assert(strcmp(status, "Over Budget") == 0);

  freeTransactionGroup(group);
  printf("✓ Budget status calculated correctly\n");
}

void test_budget_usage_percentage() {
  printf("\n=== Test: Budget Usage Percentage ===\n");

  struct TransactionGroup *group = newTransactionGroup("work", 1000000);

  long long percentage = getGroupBudgetUsagePercentage(group);
  assert(percentage == 0);

  struct Transaction *trans =
      (struct Transaction *)malloc(sizeof(struct Transaction));
  trans->amount = 500000;
  addTransactionToGroup(group, trans);

  percentage = getGroupBudgetUsagePercentage(group);
  assert(percentage == 50);

  freeTransactionGroup(group);
  printf("✓ Budget usage percentage calculated correctly\n");
}

void test_remaining_budget() {
  printf("\n=== Test: Remaining Budget ===\n");

  struct TransactionGroup *group =
      newTransactionGroup("transportation", 800000);

  long long remaining = getGroupRemainingBudget(group);
  assert(remaining == 800000);

  struct Transaction *trans =
      (struct Transaction *)malloc(sizeof(struct Transaction));
  trans->amount = 300000;
  addTransactionToGroup(group, trans);

  remaining = getGroupRemainingBudget(group);
  assert(remaining == 500000);

  freeTransactionGroup(group);
  printf("✓ Remaining budget calculated correctly\n");
}

void test_category_string_safety() {
  printf("\n=== Test: Category String Safety ===\n");

  char longCategory[100];
  memset(longCategory, 'a', 99);
  longCategory[99] = '\0';

  struct TransactionGroup *group = newTransactionGroup(longCategory, 1000000);
  assert(group != NULL);
  assert(strlen(group->category) < 50);
  assert(group->category[49] == '\0');

  freeTransactionGroup(group);
  printf("✓ Category string buffer overflow protection works\n");
}

void test_null_category_handling() {
  printf("\n=== Test: Null Category Handling ===\n");

  struct TransactionGroup *group = newTransactionGroup(NULL, 1000000);
  assert(group != NULL);
  assert(group->category[0] == '\0');

  freeTransactionGroup(group);
  printf("✓ Null category handled safely\n");
}

void test_find_transaction_by_id() {
  printf("\n=== Test: Find Transaction by ID ===\n");

  struct TransactionGroup *group = newTransactionGroup("food", 1000000);

  struct Transaction *trans1 =
      (struct Transaction *)malloc(sizeof(struct Transaction));
  trans1->id = 101;
  trans1->amount = 50000;

  struct Transaction *trans2 =
      (struct Transaction *)malloc(sizeof(struct Transaction));
  trans2->id = 102;
  trans2->amount = 30000;

  addTransactionToGroup(group, trans1);
  addTransactionToGroup(group, trans2);

  struct Transaction *found = findTransactionById(group, 101);
  assert(found != NULL);
  assert(found->id == 101);

  found = findTransactionById(group, 102);
  assert(found != NULL);
  assert(found->id == 102);

  found = findTransactionById(group, 999);
  assert(found == NULL);

  freeTransactionGroup(group);
  printf("✓ Transaction found by ID correctly\n");
}

void test_validate_transaction_for_group() {
  printf("\n=== Test: Validate Transaction for Group ===\n");

  struct TransactionGroup *group = newTransactionGroup("food", 1000000);

  struct Transaction *trans1 =
      (struct Transaction *)malloc(sizeof(struct Transaction));
  trans1->amount = 500000;
  trans1->type = TRANSACTION_EXPENSE;
  addTransactionToGroup(group, trans1);

  struct Transaction *trans2 =
      (struct Transaction *)malloc(sizeof(struct Transaction));
  trans2->amount = 400000;
  trans2->type = TRANSACTION_EXPENSE;

  int valid = validateTransactionForGroup(group, trans2);
  assert(valid == 1);

  struct Transaction *trans3 =
      (struct Transaction *)malloc(sizeof(struct Transaction));
  trans3->amount = 600000;
  trans3->type = TRANSACTION_EXPENSE;

  valid = validateTransactionForGroup(group, trans3);
  assert(valid == 0);

  free(trans2);
  free(trans3);
  freeTransactionGroup(group);
  printf("✓ Transaction validation works correctly\n");
}

int main() {
  printf("\n");
  printf("╔══════════════════════════════════════════════════════╗\n");
  printf("║      TRANSACTION GROUP TEST SUITE                   ║\n");
  printf("╚══════════════════════════════════════════════════════╝\n");

  test_new_transaction_group();
  test_transaction_group_category_string();
  test_add_transaction_to_group();
  test_remove_transaction_from_group();
  test_update_group_calculations();
  test_budget_status();
  test_budget_usage_percentage();
  test_remaining_budget();
  test_category_string_safety();
  test_null_category_handling();
  test_find_transaction_by_id();
  test_validate_transaction_for_group();

  printf("\n");
  printf("╔══════════════════════════════════════════════════════╗\n");
  printf("║      ✓ ALL TESTS PASSED                             ║\n");
  printf("╚══════════════════════════════════════════════════════╝\n");
  printf("\n");

  return 0;
}
