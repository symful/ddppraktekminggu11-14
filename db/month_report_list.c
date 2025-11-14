#include "../types/month_report_list.h"
#include "./month_report.c"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef MONTH_REPORT_LIST_C
#define MONTH_REPORT_LIST_C

void addMonthReport(struct MonthReportList *monthReportList,
                    struct MonthReport *report) {
  struct MonthReport **s = (struct MonthReport **)realloc(
      monthReportList->reports,
      (monthReportList->amount + 1) * sizeof(struct MonthReport *));
  if (s == NULL) {
    perror("Error reallocating memory");
    exit(EXIT_FAILURE);
  }
  monthReportList->reports = s;
  monthReportList->reports[monthReportList->amount++] = report;
}

struct MonthReportList *listAllMonthReports(DIR *dir) {
  struct dirent *entry;
  struct MonthReportList *monthReportList =
      (struct MonthReportList *)malloc(sizeof(struct MonthReportList));
  monthReportList->reports = NULL;
  monthReportList->amount = 0;

  if (dir == NULL) {
    printf("Direktori laporan tidak ditemukan atau tidak dapat dibuka.\n");
    return monthReportList;
  }

  char path[256];
  while ((entry = readdir(dir)) != NULL) {
    if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
      continue;
    }
    snprintf(path, sizeof(path), "%s/%s", "./reports", entry->d_name);

    struct MonthReport *report = loadMonthReport(path);
    if (report != NULL) {
      addMonthReport(monthReportList, report);
    }
  }
  return monthReportList;
}

struct MonthReportList *listUserMonthReports(DIR *dir,
                                             const char *userReportsPath) {
  struct dirent *entry;
  struct MonthReportList *monthReportList =
      (struct MonthReportList *)malloc(sizeof(struct MonthReportList));
  monthReportList->reports = NULL;
  monthReportList->amount = 0;

  if (dir == NULL) {
    printf("Direktori laporan pengguna tidak ditemukan atau tidak dapat "
           "dibuka.\n");
    return monthReportList;
  }

  char path[512];
  while ((entry = readdir(dir)) != NULL) {
    if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
      continue;
    }
    snprintf(path, sizeof(path), "%s/%s", userReportsPath, entry->d_name);

    struct MonthReport *report = loadMonthReport(path);
    if (report != NULL) {
      addMonthReport(monthReportList, report);
    }
  }
  closedir(dir);
  return monthReportList;
}

#endif
