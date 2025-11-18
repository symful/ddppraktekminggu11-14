#include "../types/include.h"
#include "./transaction_group.c"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#ifndef MONTH_REPORT_C
#define MONTH_REPORT_C

struct MonthReport *newMonthReport() {
  struct MonthReport *report =
      (struct MonthReport *)malloc(sizeof(struct MonthReport));
  if (report == NULL) {
    perror("Error allocating memory for month report");
    return NULL;
  }

  report->groups = NULL;
  report->groupsAmount = 0;
  report->totalIncome = 0;
  report->totalExpense = 0;
  report->balance = 0;

  return report;
}

void addTransactionGroupToReport(struct MonthReport *report,
                                 struct TransactionGroup *group) {
  if (report == NULL || group == NULL) {
    return;
  }

  struct TransactionGroup *temp = (struct TransactionGroup *)realloc(
      report->groups,
      (report->groupsAmount + 1) * sizeof(struct TransactionGroup));

  if (temp == NULL) {
    perror("Error reallocating memory for transaction groups");
    return;
  }

  report->groups = temp;
  report->groups[report->groupsAmount] = *group;
  report->groupsAmount++;

  updateReportCalculations(report);
}

void removeTransactionGroupFromReport(struct MonthReport *report, int index) {
  if (report == NULL || index < 0 || index >= report->groupsAmount) {
    return;
  }

  freeTransactionGroup(&report->groups[index]);

  for (int i = index; i < report->groupsAmount - 1; i++) {
    report->groups[i] = report->groups[i + 1];
  }

  report->groupsAmount--;

  if (report->groupsAmount > 0) {
    struct TransactionGroup *temp = (struct TransactionGroup *)realloc(
        report->groups, report->groupsAmount * sizeof(struct TransactionGroup));
    if (temp != NULL) {
      report->groups = temp;
    }
  } else {
    free(report->groups);
    report->groups = NULL;
  }

  updateReportCalculations(report);
}

struct TransactionGroup *
findGroupByCategory(struct MonthReport *report,
                    enum TransactionCategory category) {
  if (report == NULL) {
    return NULL;
  }

  for (int i = 0; i < report->groupsAmount; i++) {
    if (report->groups[i].category == category) {
      return &report->groups[i];
    }
  }

  return NULL;
}

void updateReportCalculations(struct MonthReport *report) {
  if (report == NULL) {
    return;
  }

  report->totalIncome = 0;
  report->totalExpense = 0;

  for (int i = 0; i < report->groupsAmount; i++) {
    struct TransactionGroup *group = &report->groups[i];
    updateGroupCalculations(group);

    for (int j = 0; j < group->transactionsAmount; j++) {
      struct Transaction *transaction = &group->transactions[j];
      if (transaction->type == TT_INCOME) {
        report->totalIncome += transaction->realCost;
      } else if (transaction->type == TT_EXPENSE) {
        report->totalExpense += transaction->realCost;
      }
    }
  }

  report->balance = report->totalIncome - report->totalExpense;
}

void freeMonthReport(struct MonthReport *report) {
  if (report == NULL) {
    return;
  }

  if (report->groups != NULL) {
    for (int i = 0; i < report->groupsAmount; i++) {
      if (report->groups[i].transactions != NULL) {
        free(report->groups[i].transactions);
      }
    }
    free(report->groups);
    report->groups = NULL;
  }

  free(report);
}

void saveMonthReport(struct MonthReport *report, const char *filename) {
  if (report == NULL || filename == NULL) {
    return;
  }

  char *dir_path = (char *)malloc(strlen(filename) + 1);
  strcpy(dir_path, filename);
  char *last_slash = strrchr(dir_path, '/');
  if (last_slash != NULL) {
    *last_slash = '\0';

    DIR *dir = opendir(dir_path);
    if (dir == NULL) {
      if (mkdir(dir_path, 0755) != 0) {
        perror("Error creating directory");
        free(dir_path);
        return;
      }
    } else {
      closedir(dir);
    }
  }
  free(dir_path);

  FILE *file = fopen(filename, "w");
  if (file == NULL) {
    perror("Error opening file for writing");
    return;
  }

  fprintf(file, "DATE=%ld\n", report->date);
  fprintf(file, "GROUPS_COUNT=%d\n", report->groupsAmount);
  fprintf(file, "TOTAL_INCOME=%lld\n", report->totalIncome);
  fprintf(file, "TOTAL_EXPENSE=%lld\n", report->totalExpense);
  fprintf(file, "BALANCE=%lld\n", report->balance);

  for (int i = 0; i < report->groupsAmount; i++) {
    struct TransactionGroup *group = &report->groups[i];
    fprintf(file, "GROUP_%d_CATEGORY=%d\n", i, (int)group->category);
    fprintf(file, "GROUP_%d_MAX_COST=%lld\n", i, group->maximumCost);
    fprintf(file, "GROUP_%d_TOTAL_REAL_COST=%lld\n", i, group->totalRealCost);
    fprintf(file, "GROUP_%d_REMAINING_COST=%lld\n", i, group->remainingCost);
    fprintf(file, "GROUP_%d_TRANSACTIONS_COUNT=%d\n", i,
            group->transactionsAmount);

    for (int j = 0; j < group->transactionsAmount; j++) {
      struct Transaction *t = &group->transactions[j];
      fprintf(file, "GROUP_%d_TRANS_%d_ID=%d\n", i, j, t->id);
      fprintf(file, "GROUP_%d_TRANS_%d_DESC=%s\n", i, j, t->description);
      fprintf(file, "GROUP_%d_TRANS_%d_NAME=%s\n", i, j, t->name);
      fprintf(file, "GROUP_%d_TRANS_%d_DATE=%ld\n", i, j, t->date);
      fprintf(file, "GROUP_%d_TRANS_%d_TYPE=%d\n", i, j, (int)t->type);
      fprintf(file, "GROUP_%d_TRANS_%d_CATEGORY=%d\n", i, j, (int)t->category);
      fprintf(file, "GROUP_%d_TRANS_%d_REAL_COST=%lld\n", i, j, t->realCost);
      fprintf(file, "GROUP_%d_TRANS_%d_MAX_COST=%lld\n", i, j, t->maximumCost);
      fprintf(file, "GROUP_%d_TRANS_%d_REST_COST=%lld\n", i, j, t->restCost);
      fprintf(file, "GROUP_%d_TRANS_%d_AMOUNT=%lld\n", i, j, t->amount);
    }
  }

  fclose(file);
}
struct MonthReport *loadMonthReport(const char *filename) {
  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    return NULL;
  }

  struct MonthReport *report = newMonthReport();
  if (report == NULL) {
    fclose(file);
    return NULL;
  }

  char line[512];
  int groupsCount = 0;

  while (fgets(line, sizeof(line), file)) {
    if (sscanf(line, "DATE=%ld", &report->date) == 1)
      continue;
    if (sscanf(line, "TOTAL_INCOME=%lld", &report->totalIncome) == 1)
      continue;
    if (sscanf(line, "TOTAL_EXPENSE=%lld", &report->totalExpense) == 1)
      continue;
    if (sscanf(line, "BALANCE=%lld", &report->balance) == 1)
      continue;

    if (sscanf(line, "GROUPS_COUNT=%d", &groupsCount) == 1) {
      if (groupsCount > 0) {
        report->groups = (struct TransactionGroup *)malloc(
            groupsCount * sizeof(struct TransactionGroup));
        report->groupsAmount = groupsCount;

        for (int i = 0; i < groupsCount; i++) {
          report->groups[i].transactions = NULL;
          report->groups[i].transactionsAmount = 0;
        }
      }
      continue;
    }

    int groupIndex;
    int tempCategory;
    if (sscanf(line, "GROUP_%d_CATEGORY=%d", &groupIndex, &tempCategory) == 2) {
      report->groups[groupIndex].category =
          (enum TransactionCategory)tempCategory;
      continue;
    }
    if (sscanf(line, "GROUP_%d_MAX_COST=%lld", &groupIndex,
               &report->groups[groupIndex].maximumCost) == 2)
      continue;
    if (sscanf(line, "GROUP_%d_TOTAL_REAL_COST=%lld", &groupIndex,
               &report->groups[groupIndex].totalRealCost) == 2)
      continue;
    if (sscanf(line, "GROUP_%d_REMAINING_COST=%lld", &groupIndex,
               &report->groups[groupIndex].remainingCost) == 2)
      continue;

    int transactionsCount;
    if (sscanf(line, "GROUP_%d_TRANSACTIONS_COUNT=%d", &groupIndex,
               &transactionsCount) == 2) {
      if (transactionsCount > 0) {
        report->groups[groupIndex].transactions = (struct Transaction *)malloc(
            transactionsCount * sizeof(struct Transaction));
        report->groups[groupIndex].transactionsAmount = transactionsCount;
      }
      continue;
    }

    int transIndex;
    if (sscanf(line, "GROUP_%d_TRANS_%d_ID=%d", &groupIndex, &transIndex,
               &report->groups[groupIndex].transactions[transIndex].id) == 3)
      continue;
    if (sscanf(
            line, "GROUP_%d_TRANS_%d_DESC=%s", &groupIndex, &transIndex,
            report->groups[groupIndex].transactions[transIndex].description) ==
        3)
      continue;
    if (sscanf(line, "GROUP_%d_TRANS_%d_NAME=%s", &groupIndex, &transIndex,
               report->groups[groupIndex].transactions[transIndex].name) == 3)
      continue;
    if (sscanf(line, "GROUP_%d_TRANS_%d_DATE=%ld", &groupIndex, &transIndex,
               &report->groups[groupIndex].transactions[transIndex].date) == 3)
      continue;

    int tempType;
    if (sscanf(line, "GROUP_%d_TRANS_%d_TYPE=%d", &groupIndex, &transIndex,
               &tempType) == 3) {
      report->groups[groupIndex].transactions[transIndex].type =
          (enum TransactionType)tempType;
      continue;
    }
    if (sscanf(line, "GROUP_%d_TRANS_%d_CATEGORY=%d", &groupIndex, &transIndex,
               &tempCategory) == 3) {
      report->groups[groupIndex].transactions[transIndex].category =
          (enum TransactionCategory)tempCategory;
      continue;
    }
    if (sscanf(
            line, "GROUP_%d_TRANS_%d_REAL_COST=%lld", &groupIndex, &transIndex,
            &report->groups[groupIndex].transactions[transIndex].realCost) == 3)
      continue;
    if (sscanf(
            line, "GROUP_%d_TRANS_%d_MAX_COST=%lld", &groupIndex, &transIndex,
            &report->groups[groupIndex].transactions[transIndex].maximumCost) ==
        3)
      continue;
    if (sscanf(
            line, "GROUP_%d_TRANS_%d_REST_COST=%lld", &groupIndex, &transIndex,
            &report->groups[groupIndex].transactions[transIndex].restCost) == 3)
      continue;
    if (sscanf(line, "GROUP_%d_TRANS_%d_AMOUNT=%lld", &groupIndex, &transIndex,
               &report->groups[groupIndex].transactions[transIndex].amount) ==
        3)
      continue;
  }

  fclose(file);
  return report;
}

void addMonthReportTransaction(struct MonthReport *monthReport,
                               struct Transaction *transaction) {
  if (monthReport == NULL || transaction == NULL) {
    return;
  }

  struct TransactionGroup *group =
      findGroupByCategory(monthReport, transaction->category);

  if (group == NULL) {

    struct TransactionGroup newGroup;
    newGroup.category = transaction->category;
    newGroup.maximumCost = 1000000;
    newGroup.transactionsAmount = 0;
    newGroup.transactions = NULL;
    newGroup.totalRealCost = 0;
    newGroup.remainingCost = newGroup.maximumCost;

    addTransactionGroupToReport(monthReport, &newGroup);
    group = &monthReport->groups[monthReport->groupsAmount - 1];
  }

  addTransactionToGroup(group, transaction);
  updateReportCalculations(monthReport);
}

void removeMonthReportTransaction(struct MonthReport *monthReport,
                                  int groupIndex, int transactionIndex) {
  if (monthReport == NULL || groupIndex < 0 ||
      groupIndex >= monthReport->groupsAmount) {
    return;
  }

  struct TransactionGroup *group = &monthReport->groups[groupIndex];
  removeTransactionFromGroup(group, transactionIndex);

  if (group->transactionsAmount == 0) {
    removeTransactionGroupFromReport(monthReport, groupIndex);
  }

  updateReportCalculations(monthReport);
}

char *generateReportFilename(time_t date) {
  char *filename = (char *)malloc(512);
  struct tm *timeinfo = localtime(&date);

  snprintf(filename, 50, "./reports/report_%04d_%02d.txt",
           timeinfo->tm_year + 1900, timeinfo->tm_mon + 1);
  return filename;
}

void saveMonthReportToFile(struct MonthReport *report) {
  char *filename = generateReportFilename(report->date);
  saveMonthReport(report, filename);
  free(filename);
}

void deleteMonthReportFile(struct MonthReport *report) {
  char *filename = generateReportFilename(report->date);
  remove(filename);
  free(filename);
}

void setBudgetForCategory(struct MonthReport *report,
                          enum TransactionCategory category, long long budget) {
  if (report == NULL) {
    return;
  }

  struct TransactionGroup *group = findGroupByCategory(report, category);

  if (group != NULL) {
    group->maximumCost = budget;
    updateGroupCalculations(group);
  } else {

    struct TransactionGroup newGroup;
    newGroup.category = category;
    newGroup.maximumCost = budget;
    newGroup.transactionsAmount = 0;
    newGroup.transactions = NULL;
    newGroup.totalRealCost = 0;
    newGroup.remainingCost = budget;

    addTransactionGroupToReport(report, &newGroup);
  }

  updateReportCalculations(report);
}

int getTotalTransactions(struct MonthReport *report) {
  if (report == NULL) {
    return 0;
  }

  int total = 0;
  for (int i = 0; i < report->groupsAmount; i++) {
    total += report->groups[i].transactionsAmount;
  }

  return total;
}

#endif
