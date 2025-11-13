#include "../types/include.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef MONTH_REPORT_C
#define MONTH_REPORT_C

struct MonthReport *newMonthReport() {
  struct MonthReport *report =
      (struct MonthReport *)malloc(sizeof(struct MonthReport));
  report->transactions = NULL;
  report->transactionsAmount = 0;
  return report;
}

void saveMonthReport(struct MonthReport *report, const char *filename) {
  FILE *file = fopen(filename, "w");
  if (file == NULL) {
    perror("Error opening file for writing");
    return;
  }

  // Save basic report info
  fprintf(file, "DATE=%ld\n", report->date);
  fprintf(file, "TRANSACTION_COUNT=%d\n", report->transactionsAmount);

  // Save each transaction
  for (int i = 0; i < report->transactionsAmount; i++) {
    struct Transaction *t = &report->transactions[i];
    fprintf(file, "TRANS_%d_ID=%d\n", i, t->id);
    fprintf(file, "TRANS_%d_DESC=%s\n", i, t->description);
    fprintf(file, "TRANS_%d_DATE=%ld\n", i, t->date);
    fprintf(file, "TRANS_%d_TYPE=%d\n", i, (int)t->type);
    fprintf(file, "TRANS_%d_CATEGORY=%d\n", i, (int)t->category);
    fprintf(file, "TRANS_%d_REAL_COST=%lld\n", i, t->realCost);
    fprintf(file, "TRANS_%d_MAX_COST=%lld\n", i, t->maximumCost);
  }

  fclose(file);
}

struct MonthReport *loadMonthReport(const char *filename) {
  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    return NULL;
  }

  struct MonthReport *report = newMonthReport();
  char line[256];
  int transactionCount = 0;

  // Read basic info
  while (fgets(line, sizeof(line), file)) {
    if (sscanf(line, "DATE=%ld", &report->date) == 1)
      continue;
    if (sscanf(line, "TRANSACTION_COUNT=%d", &transactionCount) == 1) {
      if (transactionCount > 0) {
        report->transactions = (struct Transaction *)malloc(
            transactionCount * sizeof(struct Transaction));
        report->transactionsAmount = transactionCount;
      }
      continue;
    }

    // Read transactions
    int transIndex;
    if (sscanf(line, "TRANS_%d_ID=%d", &transIndex,
               &report->transactions[transIndex].id) == 2)
      continue;
    if (sscanf(line, "TRANS_%d_DESC=%s", &transIndex,
               report->transactions[transIndex].description) == 2)
      continue;
    if (sscanf(line, "TRANS_%d_DATE=%ld", &transIndex,
               &report->transactions[transIndex].date) == 2)
      continue;

    int tempType, tempCategory;
    if (sscanf(line, "TRANS_%d_TYPE=%d", &transIndex, &tempType) == 2) {
      report->transactions[transIndex].type = (enum TransactionType)tempType;
      continue;
    }
    if (sscanf(line, "TRANS_%d_CATEGORY=%d", &transIndex, &tempCategory) == 2) {
      report->transactions[transIndex].category =
          (enum TransactionCategory)tempCategory;
      continue;
    }
    if (sscanf(line, "TRANS_%d_REAL_COST=%lld", &transIndex,
               &report->transactions[transIndex].realCost) == 2)
      continue;
    if (sscanf(line, "TRANS_%d_MAX_COST=%lld", &transIndex,
               &report->transactions[transIndex].maximumCost) == 2)
      continue;
  }

  fclose(file);
  return report;
}

void addMonthReportTransaction(struct MonthReport *monthReport,
                               struct Transaction *transaction) {
  struct Transaction *temp = (struct Transaction *)realloc(
      monthReport->transactions,
      (monthReport->transactionsAmount + 1) * sizeof(struct Transaction));
  if (temp == NULL) {
    perror("Error reallocating memory for transactions");
    return;
  }
  monthReport->transactions = temp;
  monthReport->transactions[monthReport->transactionsAmount++] = *transaction;
}

void removeMonthReportTransaction(struct MonthReport *monthReport, int index) {
  if (index < 0 || index >= monthReport->transactionsAmount) {
    return;
  }

  for (int i = index; i < monthReport->transactionsAmount - 1; i++) {
    monthReport->transactions[i] = monthReport->transactions[i + 1];
  }

  if (monthReport->transactionsAmount > 1) {
    struct Transaction *temp = (struct Transaction *)realloc(
        monthReport->transactions,
        (monthReport->transactionsAmount - 1) * sizeof(struct Transaction));
    if (temp == NULL) {
      perror("Error reallocating memory for transactions");
      return;
    }
    monthReport->transactions = temp;
  } else {
    free(monthReport->transactions);
    monthReport->transactions = NULL;
  }
  monthReport->transactionsAmount--;
}

void removeMonthReport(struct MonthReport *monthReport) {
  if (monthReport == NULL) {
    return;
  }

  if (monthReport->transactions != NULL) {
    free(monthReport->transactions);
    monthReport->transactions = NULL;
  }

  free(monthReport);
}

char *generateReportFilename(time_t date) {
  char *filename = (char *)malloc(50);
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

#endif
