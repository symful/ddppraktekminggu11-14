#include "./month_report.h"
#include <stdio.h>
#include <dirent.h>

#ifndef MONTH_REPORT_LIST_H
#define MONTH_REPORT_LIST_H

// Core list management functions
void addMonthReportToList(struct MonthReportList *list, struct MonthReport *report);
void freeMonthReportList(struct MonthReportList *list);
struct MonthReportList *createMonthReportList();

// User-aware list functions
struct MonthReportList *listUserMonthReports();
int getUserReportCount();

// Admin-only list functions  
struct MonthReportList *listAllUsersReports();
struct MonthReportList *listUserReportsAsAdmin(const char *username);
int getAllUsersReportCount();

// Utility functions
struct MonthReport *findReportInListByDate(struct MonthReportList *list, time_t date);
void sortReportListByDate(struct MonthReportList *list, int ascending);
struct MonthReportList *filterReportsByDateRange(struct MonthReportList *list, time_t startDate, time_t endDate);

#endif