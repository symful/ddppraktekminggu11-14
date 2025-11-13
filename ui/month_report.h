#include "../db/include.h"

#ifndef UI_MONTH_REPORT_H
#define UI_MONTH_REPORT_H

void showMonthReport(struct MonthReport *monthReport);
void openTransactionEditMenu(struct MonthReport *monthReport);
void openTransactionDeleteMenu(struct MonthReport *monthReport);
void openTransactionAddToReportMenu(struct MonthReport *monthReport);
void openMonthReportEditMenu(struct MonthReportList *monthReportList);
void openMonthReportDeleteMenu(struct MonthReportList *monthReportList);
void openMonthlyListMenu(struct MonthReportList *monthReportList);
void openMonthlyMenu(struct MonthReportList *monthReportList);
void openMonthReport(struct MonthReport *monthReport);
void openTransactionAddMenu(struct MonthReportList *monthReportList);
void showAllMonthReportSummary(struct MonthReportList *monthReportList);
void showMonthlyMenu(struct MonthReportList *monthReportList);
void showMonthlyList(struct MonthReportList *monthReportList);
void showMonthReportTransactions(struct MonthReport *monthReport);
void showTransactionGroupsList(struct MonthReport *monthReport);
void openTransactionViewMenu(struct MonthReport *monthReport);
void showTransactionDetails(struct TransactionGroup *group);
void openSelectReportMenu(struct MonthReportList *monthReportList);
void openCategoryBudgetMenu(struct MonthReport *monthReport);
void showMonthReportSummary2(struct MonthReport *monthReport);

extern char temp_input[100];

#endif
