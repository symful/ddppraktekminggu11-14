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
void showMonthReportSummary(struct MonthReportList *monthReportList);
void showMonthlyMenu(struct MonthReportList *monthReportList);
void showMonthlyList(struct MonthReportList *monthReportList);
void showMonthReportTransactions(struct MonthReport *monthReport);

#endif
