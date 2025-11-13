#include "./transaction_group.h"
#include <time.h>

#ifndef MONTH_REPORT_STRUCT_DEFINED
#define MONTH_REPORT_STRUCT_DEFINED

struct MonthReport {
    time_t date;
    int groupsAmount;
    struct TransactionGroup* groups;
    long long totalIncome;
    long long totalExpense;
    long long balance;
};


struct MonthReport* newMonthReport();
void addTransactionGroupToReport(struct MonthReport* report, struct TransactionGroup* group);
void removeTransactionGroupFromReport(struct MonthReport* report, int index);
struct TransactionGroup* findGroupByCategory(struct MonthReport* report, enum TransactionCategory category);
void updateReportCalculations(struct MonthReport* report);
void freeMonthReport(struct MonthReport* report);
void addMonthReportTransaction(struct MonthReport* monthReport, struct Transaction* transaction);
void removeMonthReportTransaction(struct MonthReport* monthReport, int groupIndex, int transactionIndex);
void setBudgetForCategory(struct MonthReport* report, enum TransactionCategory category, long long budget);
int getTotalTransactions(struct MonthReport* report);
void saveMonthReportToFile(struct MonthReport* report);
void deleteMonthReportFile(struct MonthReport* report);

#endif
