#include "../auth/auth.h"
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

  if (report->groupsAmount == 0) {
    free(report->groups);
    report->groups = NULL;
  } else {
    struct TransactionGroup *temp = (struct TransactionGroup *)realloc(
        report->groups, report->groupsAmount * sizeof(struct TransactionGroup));
    if (temp != NULL) {
      report->groups = temp;
    }
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
      if (group->transactions[j].type == TT_EXPENSE) {
        report->totalExpense += group->transactions[j].realCost;
      } else if (group->transactions[j].type == TT_INCOME) {
        report->totalIncome += group->transactions[j].realCost;
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

    struct stat st = {0};
    if (stat(dir_path, &st) == -1) {
      char *current = dir_path;
      char *slash = strchr(current + 1, '/');

      while (slash != NULL) {
        *slash = '\0';
        mkdir(current, 0700);
        *slash = '/';
        slash = strchr(slash + 1, '/');
      }
      mkdir(dir_path, 0700);
    }
  }
  free(dir_path);

  FILE *file = fopen(filename, "w");
  if (file == NULL) {
    perror("Error opening file for writing");
    return;
  }

  fprintf(file, "DATE:%ld\n", report->date);
  fprintf(file, "TOTAL_INCOME:%lld\n", report->totalIncome);
  fprintf(file, "TOTAL_EXPENSE:%lld\n", report->totalExpense);
  fprintf(file, "BALANCE:%lld\n", report->balance);
  fprintf(file, "GROUPS:%d\n", report->groupsAmount);

  for (int i = 0; i < report->groupsAmount; i++) {
    struct TransactionGroup *group = &report->groups[i];
    fprintf(file, "GROUP_START\n");
    fprintf(file, "CATEGORY:%d\n", group->category);
    fprintf(file, "MAX_COST:%lld\n", group->maximumCost);
    fprintf(file, "REAL_COST:%lld\n", group->totalRealCost);
    fprintf(file, "TRANSACTIONS:%d\n", group->transactionsAmount);

    for (int j = 0; j < group->transactionsAmount; j++) {
      struct Transaction *trans = &group->transactions[j];
      fprintf(file, "TRANS_START\n");
      fprintf(file, "TYPE:%d\n", trans->type);
      fprintf(file, "CATEGORY:%d\n", trans->category);
      fprintf(file, "REAL_COST:%lld\n", trans->realCost);
      fprintf(file, "DATE:%ld\n", trans->date);
      fprintf(file, "NAME:%s\n", trans->name);
      fprintf(file, "DESCRIPTION:%s\n", trans->description);
      fprintf(file, "TRANS_END\n");
    }

    fprintf(file, "GROUP_END\n");
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

  char line[1024];

  while (fgets(line, sizeof(line), file)) {
    if (strncmp(line, "DATE:", 5) == 0) {
      sscanf(line + 5, "%ld", &report->date);
    } else if (strncmp(line, "TOTAL_INCOME:", 13) == 0) {
      sscanf(line + 13, "%lld", &report->totalIncome);
    } else if (strncmp(line, "TOTAL_EXPENSE:", 14) == 0) {
      sscanf(line + 14, "%lld", &report->totalExpense);
    } else if (strncmp(line, "BALANCE:", 8) == 0) {
      sscanf(line + 8, "%lld", &report->balance);
    } else if (strncmp(line, "GROUPS:", 7) == 0) {
      int groupCount;
      sscanf(line + 7, "%d", &groupCount);

      if (groupCount > 0) {
        report->groups = (struct TransactionGroup *)malloc(
            groupCount * sizeof(struct TransactionGroup));
        report->groupsAmount = groupCount;
      }
      break;
    }
  }

  int groupIndex = 0;
  while (fgets(line, sizeof(line), file) && groupIndex < report->groupsAmount) {
    if (strncmp(line, "GROUP_START", 11) == 0) {
      struct TransactionGroup *group = &report->groups[groupIndex];
      memset(group, 0, sizeof(struct TransactionGroup));

      while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "CATEGORY:", 9) == 0) {
          sscanf(line + 9, "%d", (int *)&group->category);
        } else if (strncmp(line, "MAX_COST:", 9) == 0) {
          sscanf(line + 9, "%lld", &group->maximumCost);
        } else if (strncmp(line, "REAL_COST:", 10) == 0) {
          sscanf(line + 10, "%lld", &group->totalRealCost);
        } else if (strncmp(line, "TRANSACTIONS:", 13) == 0) {
          int transCount;
          sscanf(line + 13, "%d", &transCount);

          if (transCount > 0) {
            group->transactions = (struct Transaction *)malloc(
                transCount * sizeof(struct Transaction));
            group->transactionsAmount = transCount;

            int transIndex = 0;
            while (fgets(line, sizeof(line), file) && transIndex < transCount) {
              if (strncmp(line, "TRANS_START", 11) == 0) {
                struct Transaction *trans = &group->transactions[transIndex];
                memset(trans, 0, sizeof(struct Transaction));

                while (fgets(line, sizeof(line), file)) {
                  if (strncmp(line, "TYPE:", 5) == 0) {
                    sscanf(line + 5, "%d", (int *)&trans->type);
                  } else if (strncmp(line, "CATEGORY:", 9) == 0) {
                    sscanf(line + 9, "%d", (int *)&trans->category);
                  } else if (strncmp(line, "REAL_COST:", 10) == 0) {
                    sscanf(line + 10, "%lld", &trans->realCost);
                  } else if (strncmp(line, "DATE:", 5) == 0) {
                    sscanf(line + 5, "%ld", &trans->date);
                  } else if (strncmp(line, "NAME:", 5) == 0) {
                    sscanf(line + 5, " %[^\n]", trans->name);
                  } else if (strncmp(line, "DESCRIPTION:", 12) == 0) {
                    sscanf(line + 12, " %[^\n]", trans->description);
                  } else if (strncmp(line, "TRANS_END", 9) == 0) {
                    transIndex++;
                    break;
                  }
                }
              }
            }
          }
          break;
        }
      }
      groupIndex++;
    }
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
    struct TransactionGroup newGroup = {};
    newGroup.category = transaction->category;
    newGroup.maximumCost = 1000000;
    newGroup.totalRealCost = 0;
    newGroup.transactions = NULL;
    newGroup.transactionsAmount = 0;

    addTransactionToGroup(&newGroup, transaction);
    addTransactionGroupToReport(monthReport, &newGroup);
  } else {
    addTransactionToGroup(group, transaction);
  }

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
  char *filename = (char *)malloc(256);
  if (filename == NULL) {
    return NULL;
  }

  struct tm *timeinfo = localtime(&date);

  // User-aware filename generation
  if (currentUser != NULL && !currentUser->isAdmin) {
    snprintf(filename, 256, "%s/%s/reports/report_%04d_%02d.txt", USERS_DIR,
             currentUser->username, timeinfo->tm_year + 1900,
             timeinfo->tm_mon + 1);
  } else {
    snprintf(filename, 256, "./reports/report_%04d_%02d.txt",
             timeinfo->tm_year + 1900, timeinfo->tm_mon + 1);
  }

  return filename;
}

void saveMonthReportToFile(struct MonthReport *report) {
  if (report == NULL) {
    return;
  }

  char *filename = generateReportFilename(report->date);
  if (filename != NULL) {
    saveMonthReport(report, filename);
    free(filename);
  }
}

void deleteMonthReportFile(struct MonthReport *report) {
  if (report == NULL) {
    return;
  }

  char *filename = generateReportFilename(report->date);
  if (filename != NULL) {
    remove(filename);
    free(filename);
  }
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
    struct TransactionGroup newGroup = {};
    newGroup.category = category;
    newGroup.maximumCost = budget;
    newGroup.totalRealCost = 0;
    newGroup.transactions = NULL;
    newGroup.transactionsAmount = 0;

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

// User-aware utility functions from auth system
struct MonthReport *loadUserMonthReport(time_t date) {
  if (currentUser == NULL || currentUser->isAdmin) {
    return NULL;
  }

  char *filename = generateReportFilename(date);
  if (filename == NULL) {
    return NULL;
  }

  struct MonthReport *report = loadMonthReport(filename);
  free(filename);
  return report;
}

int userReportExists(time_t date) {
  if (currentUser == NULL || currentUser->isAdmin) {
    return 0;
  }

  char *filename = generateReportFilename(date);
  if (filename == NULL) {
    return 0;
  }

  FILE *file = fopen(filename, "r");
  free(filename);

  if (file != NULL) {
    fclose(file);
    return 1;
  }

  return 0;
}

char *getUserReportsDirectory() {
  if (currentUser == NULL || currentUser->isAdmin) {
    return NULL;
  }

  static char reportsDir[256];
  snprintf(reportsDir, sizeof(reportsDir), "%s/%s/reports", USERS_DIR,
           currentUser->username);
  return reportsDir;
}

void setUserCategoryBudget(struct MonthReport *report,
                           enum TransactionCategory category,
                           long long budget) {
  if (report == NULL || currentUser == NULL || currentUser->isAdmin) {
    return;
  }

  setBudgetForCategory(report, category, budget);
  saveMonthReportToFile(report);
}

void addUserMonthReportTransaction(struct MonthReport *monthReport,
                                   struct Transaction *transaction) {
  if (monthReport == NULL || transaction == NULL || currentUser == NULL ||
      currentUser->isAdmin) {
    return;
  }

  addMonthReportTransaction(monthReport, transaction);
  saveMonthReportToFile(monthReport);
}

void removeUserMonthReportTransaction(struct MonthReport *monthReport,
                                      int groupIndex, int transactionIndex) {
  if (monthReport == NULL || currentUser == NULL || currentUser->isAdmin) {
    return;
  }

  removeMonthReportTransaction(monthReport, groupIndex, transactionIndex);
  saveMonthReportToFile(monthReport);
}

void updateUserReportDate(struct MonthReport *report, time_t newDate) {
  if (report == NULL || currentUser == NULL || currentUser->isAdmin) {
    return;
  }

  deleteMonthReportFile(report);
  report->date = newDate;
  saveMonthReportToFile(report);
}

#endif
