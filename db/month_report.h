#include "../types/include.h"
#include <stdio.h>
#include <dirent.h>

#ifndef DB_MONTH_REPORT_H
#define DB_MONTH_REPORT_H

// Core database functions
void saveMonthReport(struct MonthReport* report, const char *filename);
struct MonthReport *loadMonthReport(const char *filename);
struct MonthReport *newMonthReport();
void addMonthReportTransaction(struct MonthReport *monthReport, struct Transaction *transaction);
void removeMonthReportTransaction(struct MonthReport *monthReport, int groupIndex, int transactionIndex);
void removeMonthReport(struct MonthReport *monthReport);
char *generateReportFilename(time_t date);
void saveMonthReportToFile(struct MonthReport *report);
void deleteMonthReportFile(struct MonthReport *report);
void addTransactionGroupToReport(struct MonthReport *report, struct TransactionGroup *group);
void removeTransactionGroupFromReport(struct MonthReport *report, int index);
struct TransactionGroup *findGroupByCategory(struct MonthReport *report, enum TransactionCategory category);
void updateReportCalculations(struct MonthReport *report);
void freeMonthReport(struct MonthReport *report);
void setBudgetForCategory(struct MonthReport *report, enum TransactionCategory category, long long budget);
int getTotalTransactions(struct MonthReport *report);

// User-aware functions (moved from auth system)
struct MonthReport *loadUserMonthReport(time_t date);
int userReportExists(time_t date);
char *getUserReportsDirectory();
void setUserCategoryBudget(struct MonthReport *report, enum TransactionCategory category, long long budget);
void addUserMonthReportTransaction(struct MonthReport *monthReport, struct Transaction *transaction);
void removeUserMonthReportTransaction(struct MonthReport *monthReport, int groupIndex, int transactionIndex);
void updateUserReportDate(struct MonthReport *report, time_t newDate);

// List management functions
struct MonthReportList *listUserMonthReports(DIR *dir, const char *dirPath);

#endif