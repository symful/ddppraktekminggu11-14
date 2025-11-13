#include "../types/include.h"
#include <stdio.h>
#include <dirent.h>

#ifndef MONTH_REPORT_H
#define MONTH_REPORT_H

void saveMonthReport(struct MonthReport* report, const char *filename);
struct MonthReport *loadMonthReport(const char *filename);
struct MonthReport *newMonthReport();
void addMonthReportTransaction(struct MonthReport *monthReport, struct Transaction *transaction);
void removeMonthReportTransaction(struct MonthReport *monthReport, int index);
void removeMonthReport(struct MonthReport *monthReport);
char *generateReportFilename(time_t date);
void saveMonthReportToFile(struct MonthReport *report);
void deleteMonthReportFile(struct MonthReport *report);

#endif
