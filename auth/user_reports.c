#include "../db/include.c"
#include "../utils/env_loader.c"
#include "auth.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>

#ifndef USER_REPORTS_C
#define USER_REPORTS_C

char *generateUserReportFilename(time_t date) {
  if (currentUser == NULL || currentUser->isAdmin) {
    return NULL;
  }

  char *filename = (char *)malloc(512);
  if (filename == NULL) {
    return NULL;
  }

  struct tm *timeinfo = localtime(&date);
  snprintf(filename, 512, "%s/%s/reports/report_%04d_%02d.txt", USERS_DIR,
           currentUser->username, timeinfo->tm_year + 1900,
           timeinfo->tm_mon + 1);
  return filename;
}

#ifdef USER_AWARE_REPORTS

void saveUserAwareMonthReportToFile(struct MonthReport *report) {
  char *filename = generateUserReportFilename(report->date);
  saveMonthReport(report, filename);
  free(filename);
}

void deleteUserAwareMonthReportFile(struct MonthReport *report) {
  char *filename = generateUserReportFilename(report->date);
  remove(filename);
  free(filename);
}

#endif

#endif
