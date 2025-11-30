#include "../auth/auth.h"
#include "../types/include.c"
#include "../utils/directory.c"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

#ifndef MONTH_REPORT_C
#define MONTH_REPORT_C

void updateReportCalculations(struct MonthReport *report);

struct MonthReport *newMonthReport() {
  struct MonthReport *report =
      (struct MonthReport *)malloc(sizeof(struct MonthReport));
  if (report == NULL) {
    perror("Error allocating memory for month report");
    return NULL;
  }

  report->groups = NULL;
  report->groupCount = 0;
  report->totalIncome = 0;
  report->totalExpenses = 0;
  report->balance = 0;
  report->date = time(NULL);

  return report;
}

void addTransactionGroupToReport(struct MonthReport *report,
                                 struct TransactionGroup *group) {
  if (report == NULL || group == NULL) {
    return;
  }

  struct TransactionGroup **newGroups = (struct TransactionGroup **)realloc(
      report->groups,
      (report->groupCount + 1) * sizeof(struct TransactionGroup *));

  if (newGroups == NULL) {
    perror("Error reallocating memory for transaction groups");
    return;
  }

  report->groups = newGroups;
  report->groups[report->groupCount] = group;
  report->groupCount++;

  updateReportCalculations(report);
}

void removeTransactionGroupFromReport(struct MonthReport *report, int index) {
  if (report == NULL || index < 0 || index >= report->groupCount) {
    return;
  }

  if (report->groups[index] != NULL) {

    free(report->groups[index]);
  }

  for (int i = index; i < report->groupCount - 1; i++) {
    report->groups[i] = report->groups[i + 1];
  }

  report->groupCount--;

  if (report->groupCount == 0) {
    free(report->groups);
    report->groups = NULL;
  } else {
    struct TransactionGroup **newGroups = (struct TransactionGroup **)realloc(
        report->groups, report->groupCount * sizeof(struct TransactionGroup *));
    if (newGroups != NULL) {
      report->groups = newGroups;
    }
  }

  updateReportCalculations(report);
}

struct TransactionGroup *findGroupByCategory(struct MonthReport *report,
                                             const char *category) {
  if (report == NULL || category == NULL) {
    return NULL;
  }

  for (int i = 0; i < report->groupCount; i++) {
    if (report->groups[i] != NULL &&
        strcmp(report->groups[i]->category, category) == 0) {
      return report->groups[i];
    }
  }

  return NULL;
}

void updateReportCalculations(struct MonthReport *report) {
  if (report == NULL) {
    return;
  }

  report->totalIncome = 0;
  report->totalExpenses = 0;

  for (int i = 0; i < report->groupCount; i++) {
    if (report->groups[i] != NULL) {
      for (int j = 0; j < report->groups[i]->transactionCount; j++) {
        if (report->groups[i]->transactions[j] != NULL) {
          if (report->groups[i]->transactions[j]->type == TRANSACTION_INCOME) {
            report->totalIncome += report->groups[i]->transactions[j]->amount;
          } else {
            report->totalExpenses += report->groups[i]->transactions[j]->amount;
          }
        }
      }
    }
  }

  report->balance = report->totalIncome - report->totalExpenses;
}

void freeMonthReport(struct MonthReport *report) {
  if (report == NULL) {
    return;
  }

  if (report->groups != NULL) {
    for (int i = 0; i < report->groupCount; i++) {
      if (report->groups[i] != NULL) {

        free(report->groups[i]);
      }
    }
    free(report->groups);
  }

  free(report);
}

void setBudgetForCategory(struct MonthReport *report, const char *category,
                          long long budget) {
  if (report == NULL || category == NULL) {
    return;
  }

  struct TransactionGroup *group = findGroupByCategory(report, category);
  if (group != NULL) {
    group->budget = budget;
  } else {

    struct TransactionGroup *newGroup =
        (struct TransactionGroup *)malloc(sizeof(struct TransactionGroup));
    if (newGroup != NULL) {
      strncpy(newGroup->category, category, sizeof(newGroup->category) - 1);
      newGroup->category[sizeof(newGroup->category) - 1] = '\0';
      newGroup->budget = budget;
      newGroup->totalAmount = 0;
      newGroup->transactions = NULL;
      newGroup->transactionCount = 0;
      addTransactionGroupToReport(report, newGroup);
    }
  }
}

int getTotalTransactions(struct MonthReport *report) {
  if (report == NULL) {
    return 0;
  }

  int totalTransactions = 0;
  for (int i = 0; i < report->groupCount; i++) {
    if (report->groups[i] != NULL) {
      totalTransactions += report->groups[i]->transactionCount;
    }
  }

  return totalTransactions;
}

void saveMonthReportToPath(struct MonthReport *report, const char *filepath) {
  if (report == NULL || filepath == NULL) {
    return;
  }

  FILE *file = fopen(filepath, "w");
  if (file == NULL) {
    perror("Error opening file for writing");
    return;
  }

  fprintf(file, "MONTH_REPORT\n");
  fprintf(file, "DATE=%ld\n", report->date);
  fprintf(file, "TOTAL_INCOME=%lld\n", report->totalIncome);
  fprintf(file, "TOTAL_EXPENSES=%lld\n", report->totalExpenses);
  fprintf(file, "BALANCE=%lld\n", report->balance);
  fprintf(file, "GROUP_COUNT=%d\n", report->groupCount);

  for (int i = 0; i < report->groupCount; i++) {
    if (report->groups[i] != NULL) {
      fprintf(file, "GROUP_START\n");
      fprintf(file, "CATEGORY=%s\n", report->groups[i]->category);
      fprintf(file, "BUDGET=%lld\n", report->groups[i]->budget);
      fprintf(file, "TOTAL_AMOUNT=%lld\n", report->groups[i]->totalAmount);
      fprintf(file, "TRANSACTION_COUNT=%d\n",
              report->groups[i]->transactionCount);

      for (int j = 0; j < report->groups[i]->transactionCount; j++) {
        if (report->groups[i]->transactions[j] != NULL) {
          struct Transaction *trans = report->groups[i]->transactions[j];
          fprintf(file, "TRANSACTION_START\n");
          fprintf(file, "ID=%d\n", trans->id);
          fprintf(file, "AMOUNT=%lld\n", trans->amount);
          fprintf(file, "TYPE=%d\n", trans->type);
          fprintf(file, "CATEGORY=%s\n", trans->category);
          fprintf(file, "STATUS=%d\n", trans->status);
          fprintf(file, "DATE=%ld\n", trans->date);
          fprintf(file, "NAME=%s\n", trans->name);
          fprintf(file, "DESCRIPTION=%s\n", trans->description);
          fprintf(file, "TRANSACTION_END\n");
        }
      }
      fprintf(file, "GROUP_END\n");
    }
  }

  fclose(file);
}

struct MonthReport *loadMonthReportFromPath(const char *filepath) {
  if (filepath == NULL) {
    return NULL;
  }

  FILE *file = fopen(filepath, "r");
  if (file == NULL) {
    return NULL;
  }

  char line[512];
  if (fgets(line, sizeof(line), file) == NULL ||
      strncmp(line, "MONTH_REPORT", 12) != 0) {
    fclose(file);
    return NULL;
  }

  struct MonthReport *report = newMonthReport();
  if (report == NULL) {
    fclose(file);
    return NULL;
  }

  while (fgets(line, sizeof(line), file) != NULL) {
    if (strncmp(line, "DATE=", 5) == 0) {
      report->date = strtol(line + 5, NULL, 10);
    } else if (strncmp(line, "TOTAL_INCOME=", 13) == 0) {
      report->totalIncome = strtoll(line + 13, NULL, 10);
    } else if (strncmp(line, "TOTAL_EXPENSES=", 15) == 0) {
      report->totalExpenses = strtoll(line + 15, NULL, 10);
    } else if (strncmp(line, "BALANCE=", 8) == 0) {
      report->balance = strtoll(line + 8, NULL, 10);
    } else if (strncmp(line, "GROUP_COUNT=", 12) == 0) {

    } else if (strncmp(line, "GROUP_START", 11) == 0) {

      struct TransactionGroup *group =
          (struct TransactionGroup *)malloc(sizeof(struct TransactionGroup));
      if (group == NULL) {
        freeMonthReport(report);
        fclose(file);
        return NULL;
      }

      group->transactions = NULL;
      group->transactionCount = 0;
      group->budget = 0;
      group->totalAmount = 0;

      while (fgets(line, sizeof(line), file) != NULL &&
             strncmp(line, "GROUP_END", 9) != 0) {
        if (strncmp(line, "CATEGORY=", 9) == 0) {
          strncpy(group->category, line + 9, sizeof(group->category) - 1);
          group->category[sizeof(group->category) - 1] = '\0';
          char *newline = strchr(group->category, '\n');
          if (newline)
            *newline = '\0';
        } else if (strncmp(line, "BUDGET=", 7) == 0) {
          group->budget = strtoll(line + 7, NULL, 10);
        } else if (strncmp(line, "TOTAL_AMOUNT=", 13) == 0) {
          group->totalAmount = strtoll(line + 13, NULL, 10);
        } else if (strncmp(line, "TRANSACTION_COUNT=", 18) == 0) {

        } else if (strncmp(line, "TRANSACTION_START", 17) == 0) {

          struct Transaction *trans =
              (struct Transaction *)malloc(sizeof(struct Transaction));
          if (trans == NULL) {
            free(group);
            freeMonthReport(report);
            fclose(file);
            return NULL;
          }

          memset(trans->name, 0, sizeof(trans->name));
          memset(trans->description, 0, sizeof(trans->description));

          while (fgets(line, sizeof(line), file) != NULL &&
                 strncmp(line, "TRANSACTION_END", 15) != 0) {
            if (strncmp(line, "ID=", 3) == 0) {
              trans->id = atoi(line + 3);
            } else if (strncmp(line, "AMOUNT=", 7) == 0) {
              trans->amount = strtoll(line + 7, NULL, 10);
            } else if (strncmp(line, "TYPE=", 5) == 0) {
              trans->type = (enum TransactionType)atoi(line + 5);
            } else if (strncmp(line, "CATEGORY=", 9) == 0) {
              strncpy(trans->category, line + 9, sizeof(trans->category) - 1);
              trans->category[sizeof(trans->category) - 1] = '\0';
              char *newline = strchr(trans->category, '\n');
              if (newline)
                *newline = '\0';
            } else if (strncmp(line, "STATUS=", 7) == 0) {
              trans->status = (enum TransactionStatus)atoi(line + 7);
            } else if (strncmp(line, "DATE=", 5) == 0) {
              trans->date = strtol(line + 5, NULL, 10);
            } else if (strncmp(line, "NAME=", 5) == 0) {
              char *newline = strchr(line + 5, '\n');
              if (newline)
                *newline = '\0';
              strncpy(trans->name, line + 5, sizeof(trans->name) - 1);
            } else if (strncmp(line, "DESCRIPTION=", 12) == 0) {
              char *newline = strchr(line + 12, '\n');
              if (newline)
                *newline = '\0';
              strncpy(trans->description, line + 12,
                      sizeof(trans->description) - 1);
            }
          }

          struct Transaction **newTransactions = (struct Transaction **)realloc(
              group->transactions,
              (group->transactionCount + 1) * sizeof(struct Transaction *));
          if (newTransactions != NULL) {
            group->transactions = newTransactions;
            group->transactions[group->transactionCount] = trans;
            group->transactionCount++;
          } else {
            free(trans);
          }
        }
      }

      addTransactionGroupToReport(report, group);
    }
  }

  fclose(file);
  return report;
}

void deleteMonthReportAtPath(const char *filepath) {
  if (filepath == NULL) {
    return;
  }

  if (remove(filepath) != 0) {
    perror("Error deleting report file");
  }
}

char *getUserReportsDirectory() {
  if (currentUser == NULL) {
    return NULL;
  }

  static char reportsDir[512];
  if (currentUser->isAdmin) {

    snprintf(reportsDir, sizeof(reportsDir), "%s", USERS_DIR);
  } else {
    snprintf(reportsDir, sizeof(reportsDir), "%s/%s/reports", USERS_DIR,
             currentUser->username);
  }

  return reportsDir;
}

char *generateUserReportFilepath(time_t date) {
  if (currentUser == NULL || currentUser->isAdmin) {
    return NULL;
  }

  char *filepath = (char *)malloc(512);
  if (filepath == NULL) {
    return NULL;
  }

  struct tm *timeinfo = localtime(&date);
  snprintf(filepath, 512, "%s/%s/reports/report_%04d_%02d.txt", USERS_DIR,
           currentUser->username, timeinfo->tm_year + 1900,
           timeinfo->tm_mon + 1);

  return filepath;
}

int ensureUserReportsDirectoryExists() {
  if (currentUser == NULL || currentUser->isAdmin) {
    return 0;
  }

  return ensureUserReportsDirectory(currentUser->username);
}

struct MonthReport *loadUserMonthReport(time_t date) {
  if (currentUser == NULL || currentUser->isAdmin) {
    return NULL;
  }

  char *filepath = generateUserReportFilepath(date);
  if (filepath == NULL) {
    return NULL;
  }

  struct MonthReport *report = loadMonthReportFromPath(filepath);
  free(filepath);

  return report;
}

void saveUserMonthReport(struct MonthReport *report) {
  if (report == NULL || currentUser == NULL || currentUser->isAdmin) {
    return;
  }

  if (!ensureUserReportsDirectoryExists()) {
    printf("Error: Could not create user reports directory\n");
    return;
  }

  char *filepath = generateUserReportFilepath(report->date);
  if (filepath == NULL) {
    return;
  }

  saveMonthReportToPath(report, filepath);
  free(filepath);
}

void deleteUserMonthReport(struct MonthReport *report) {
  if (report == NULL || currentUser == NULL || currentUser->isAdmin) {
    return;
  }

  char *filepath = generateUserReportFilepath(report->date);
  if (filepath == NULL) {
    return;
  }

  deleteMonthReportAtPath(filepath);
  free(filepath);
}

int userReportExists(time_t date) {
  if (currentUser == NULL || currentUser->isAdmin) {
    return 0;
  }

  char *filepath = generateUserReportFilepath(date);
  if (filepath == NULL) {
    return 0;
  }

  FILE *file = fopen(filepath, "r");
  free(filepath);

  if (file != NULL) {
    fclose(file);
    return 1;
  }

  return 0;
}

void addUserMonthReportTransaction(struct MonthReport *monthReport,
                                   struct Transaction *transaction) {
  if (monthReport == NULL || transaction == NULL || currentUser == NULL ||
      currentUser->isAdmin) {
    return;
  }

  struct TransactionGroup *group =
      findGroupByCategory(monthReport, transaction->category);
  if (group == NULL) {

    group = (struct TransactionGroup *)malloc(sizeof(struct TransactionGroup));
    if (group == NULL) {
      return;
    }
    strncpy(group->category, transaction->category,
            sizeof(group->category) - 1);
    group->category[sizeof(group->category) - 1] = '\0';
    group->budget = 0;
    group->totalAmount = 0;
    group->transactions = NULL;
    group->transactionCount = 0;
    addTransactionGroupToReport(monthReport, group);
  }

  struct Transaction **newTransactions = (struct Transaction **)realloc(
      group->transactions,
      (group->transactionCount + 1) * sizeof(struct Transaction *));
  if (newTransactions != NULL) {
    group->transactions = newTransactions;
    group->transactions[group->transactionCount] = transaction;
    group->transactionCount++;
    group->totalAmount += transaction->amount;

    updateReportCalculations(monthReport);
    saveUserMonthReport(monthReport);
  }
}

void removeUserMonthReportTransaction(struct MonthReport *monthReport,
                                      int groupIndex, int transactionIndex) {
  if (monthReport == NULL || currentUser == NULL || currentUser->isAdmin) {
    return;
  }

  if (groupIndex < 0 || groupIndex >= monthReport->groupCount) {
    return;
  }

  struct TransactionGroup *group = monthReport->groups[groupIndex];
  if (group == NULL || transactionIndex < 0 ||
      transactionIndex >= group->transactionCount) {
    return;
  }

  struct Transaction *removedTransaction =
      group->transactions[transactionIndex];
  group->totalAmount -= removedTransaction->amount;

  free(removedTransaction);

  for (int i = transactionIndex; i < group->transactionCount - 1; i++) {
    group->transactions[i] = group->transactions[i + 1];
  }

  group->transactionCount--;

  if (group->transactionCount == 0) {
    free(group->transactions);
    group->transactions = NULL;
  } else {
    struct Transaction **newTransactions = (struct Transaction **)realloc(
        group->transactions,
        group->transactionCount * sizeof(struct Transaction *));
    if (newTransactions != NULL) {
      group->transactions = newTransactions;
    }
  }

  updateReportCalculations(monthReport);
  saveUserMonthReport(monthReport);
}

void setUserCategoryBudget(struct MonthReport *report, const char *category,
                           long long budget) {
  if (report == NULL || currentUser == NULL || currentUser->isAdmin) {
    return;
  }

  setBudgetForCategory(report, category, budget);
  saveUserMonthReport(report);
}

void updateUserReportDate(struct MonthReport *report, time_t newDate) {
  if (report == NULL || currentUser == NULL || currentUser->isAdmin) {
    return;
  }

  deleteUserMonthReport(report);
  report->date = newDate;
  saveUserMonthReport(report);
}

char *generateAdminReportFilepath(const char *username, time_t date) {
  if (currentUser == NULL || !currentUser->isAdmin || username == NULL) {
    return NULL;
  }

  char *filepath = (char *)malloc(512);
  if (filepath == NULL) {
    return NULL;
  }

  struct tm *timeinfo = localtime(&date);
  snprintf(filepath, 512, "%s/%s/reports/report_%04d_%02d.txt", USERS_DIR,
           username, timeinfo->tm_year + 1900, timeinfo->tm_mon + 1);

  return filepath;
}

struct MonthReport *loadAdminMonthReport(const char *username, time_t date) {
  if (currentUser == NULL || !currentUser->isAdmin || username == NULL) {
    return NULL;
  }

  char *filepath = generateAdminReportFilepath(username, date);
  if (filepath == NULL) {
    return NULL;
  }

  struct MonthReport *report = loadMonthReportFromPath(filepath);
  free(filepath);

  return report;
}

int adminReportExists(const char *username, time_t date) {
  if (currentUser == NULL || !currentUser->isAdmin || username == NULL) {
    return 0;
  }

  char *filepath = generateAdminReportFilepath(username, date);
  if (filepath == NULL) {
    return 0;
  }

  FILE *file = fopen(filepath, "r");
  free(filepath);

  if (file != NULL) {
    fclose(file);
    return 1;
  }

  return 0;
}

#endif
