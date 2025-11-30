#include "../types/include.h"
#include <stdio.h>
#include <dirent.h>

#ifndef DB_MONTH_REPORT_H
#define DB_MONTH_REPORT_H

struct MonthReport *newMonthReport();
void addTransactionGroupToReport(struct MonthReport *report, struct TransactionGroup *group);
void removeTransactionGroupFromReport(struct MonthReport *report, int index);
struct TransactionGroup *findGroupByCategory(struct MonthReport *report, const char* category);
void updateReportCalculations(struct MonthReport *report);
void freeMonthReport(struct MonthReport *report);
void setBudgetForCategory(struct MonthReport *report, const char* category, long long budget);
int getTotalTransactions(struct MonthReport *report);

void saveMonthReportToPath(struct MonthReport *report, const char *filepath);
struct MonthReport *loadMonthReportFromPath(const char *filepath);
void deleteMonthReportAtPath(const char *filepath);


char *getUserReportsDirectory();
char *generateUserReportFilepath(time_t date);
int ensureUserReportsDirectoryExists();


struct MonthReport *loadUserMonthReport(time_t date);
void saveUserMonthReport(struct MonthReport *report);
void deleteUserMonthReport(struct MonthReport *report);
int userReportExists(time_t date);


void addUserMonthReportTransaction(struct MonthReport *monthReport, struct Transaction *transaction);
void removeUserMonthReportTransaction(struct MonthReport *monthReport, int groupIndex, int transactionIndex);
void setUserCategoryBudget(struct MonthReport *report, const char* category, long long budget);
void updateUserReportDate(struct MonthReport *report, time_t newDate);


struct MonthReport *loadAdminMonthReport(const char *username, time_t date);
char *generateAdminReportFilepath(const char *username, time_t date);
int adminReportExists(const char *username, time_t date);


#define saveUserMonthReportToFile(report) saveUserMonthReport(report)

#endif
