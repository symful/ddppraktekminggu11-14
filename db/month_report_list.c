#include "../types/month_report_list.h"
#include "../auth/auth.h"
#include "./month_report.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

#ifndef MONTH_REPORT_LIST_C
#define MONTH_REPORT_LIST_C

struct MonthReportList *createMonthReportList() {
  struct MonthReportList *list =
      (struct MonthReportList *)malloc(sizeof(struct MonthReportList));
  if (list == NULL) {
    perror("Error allocating memory for month report list");
    return NULL;
  }

  list->reports = NULL;
  list->count = 0;
  return list;
}

void addMonthReportToList(struct MonthReportList *list,
                          struct MonthReport *report) {
  if (list == NULL || report == NULL) {
    return;
  }

  struct MonthReport **newReports = (struct MonthReport **)realloc(
      list->reports, (list->count + 1) * sizeof(struct MonthReport *));

  if (newReports == NULL) {
    perror("Error reallocating memory for month report list");
    return;
  }

  list->reports = newReports;
  list->reports[list->count] = report;
  list->count++;
}

void freeMonthReportList(struct MonthReportList *list) {
  if (list == NULL) {
    return;
  }

  if (list->reports != NULL) {
    for (int i = 0; i < list->count; i++) {
      if (list->reports[i] != NULL) {
        freeMonthReport(list->reports[i]);
      }
    }
    free(list->reports);
  }

  free(list);
}

struct MonthReportList *listUserMonthReports() {
  if (currentUser == NULL || currentUser->isAdmin) {
    printf(
        "Error: No valid user session or admin trying to use user function\n");
    return NULL;
  }

  char *userReportsDir = getUserReportsDirectory();
  if (userReportsDir == NULL) {
    printf("Error: Cannot get user reports directory\n");
    return NULL;
  }

  DIR *dir = opendir(userReportsDir);
  if (dir == NULL) {

    return createMonthReportList();
  }

  struct MonthReportList *list = createMonthReportList();
  if (list == NULL) {
    closedir(dir);
    return NULL;
  }

  struct dirent *entry;
  char filepath[1024];

  while ((entry = readdir(dir)) != NULL) {

    if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
      continue;
    }

    if (strstr(entry->d_name, "report_") != entry->d_name ||
        strstr(entry->d_name, ".txt") == NULL) {
      continue;
    }

    if (snprintf(filepath, sizeof(filepath), "%s/%s", userReportsDir,
                 entry->d_name) >= (int)sizeof(filepath)) {
      printf("Warning: Path too long for file %s, skipping.\n", entry->d_name);
      continue;
    }

    struct MonthReport *report = loadMonthReportFromPath(filepath);
    if (report != NULL) {
      addMonthReportToList(list, report);
    }
  }

  closedir(dir);
  return list;
}

int getUserReportCount() {
  if (currentUser == NULL || currentUser->isAdmin) {
    return 0;
  }

  char *userReportsDir = getUserReportsDirectory();
  if (userReportsDir == NULL) {
    return 0;
  }

  DIR *dir = opendir(userReportsDir);
  if (dir == NULL) {
    return 0;
  }

  int count = 0;
  struct dirent *entry;

  while ((entry = readdir(dir)) != NULL) {
    if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
      continue;
    }

    if (strstr(entry->d_name, "report_") == entry->d_name &&
        strstr(entry->d_name, ".txt") != NULL) {
      count++;
    }
  }

  closedir(dir);
  return count;
}

struct MonthReportList *listAllUsersReports() {
  if (currentUser == NULL || !currentUser->isAdmin) {
    printf("Error: Admin access required\n");
    return NULL;
  }

  struct MonthReportList *allReports = createMonthReportList();
  if (allReports == NULL) {
    return NULL;
  }

  DIR *usersDir = opendir(USERS_DIR);
  if (usersDir == NULL) {
    printf("Error: Cannot open users directory\n");
    freeMonthReportList(allReports);
    return NULL;
  }

  struct dirent *userEntry;
  char userReportsPath[1024];

  while ((userEntry = readdir(usersDir)) != NULL) {
    if (strcmp(userEntry->d_name, ".") == 0 ||
        strcmp(userEntry->d_name, "..") == 0) {
      continue;
    }

    snprintf(userReportsPath, sizeof(userReportsPath), "%s/%s", USERS_DIR,
             userEntry->d_name);
    struct stat statbuf;
    if (stat(userReportsPath, &statbuf) != 0 || !S_ISDIR(statbuf.st_mode)) {
      continue;
    }

    snprintf(userReportsPath, sizeof(userReportsPath), "%s/%s/reports",
             USERS_DIR, userEntry->d_name);
    DIR *reportsDir = opendir(userReportsPath);
    if (reportsDir == NULL) {
      continue;
    }

    struct dirent *reportEntry;
    char reportFilePath[1024];

    while ((reportEntry = readdir(reportsDir)) != NULL) {
      if (strcmp(reportEntry->d_name, ".") == 0 ||
          strcmp(reportEntry->d_name, "..") == 0) {
        continue;
      }

      if (strstr(reportEntry->d_name, "report_") != reportEntry->d_name ||
          strstr(reportEntry->d_name, ".txt") == NULL) {
        continue;
      }

      snprintf(reportFilePath, sizeof(reportFilePath), "%s/%s", userReportsPath,
               reportEntry->d_name);

      struct MonthReport *report = loadMonthReportFromPath(reportFilePath);
      if (report != NULL) {
        addMonthReportToList(allReports, report);
      }
    }

    closedir(reportsDir);
  }

  closedir(usersDir);
  return allReports;
}

struct MonthReportList *listUserReportsAsAdmin(const char *username) {
  if (currentUser == NULL || !currentUser->isAdmin || username == NULL) {
    printf("Error: Admin access required and valid username needed\n");
    return NULL;
  }

  char userReportsDir[512];
  snprintf(userReportsDir, sizeof(userReportsDir), "%s/%s/reports", USERS_DIR,
           username);

  DIR *dir = opendir(userReportsDir);
  if (dir == NULL) {

    return createMonthReportList();
  }

  struct MonthReportList *list = createMonthReportList();
  if (list == NULL) {
    closedir(dir);
    return NULL;
  }

  struct dirent *entry;
  char filepath[1024];

  while ((entry = readdir(dir)) != NULL) {
    if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
      continue;
    }

    if (strstr(entry->d_name, "report_") != entry->d_name ||
        strstr(entry->d_name, ".txt") == NULL) {
      continue;
    }

    snprintf(filepath, sizeof(filepath), "%s/%s", userReportsDir,
             entry->d_name);

    struct MonthReport *report = loadMonthReportFromPath(filepath);
    if (report != NULL) {
      addMonthReportToList(list, report);
    }
  }

  closedir(dir);
  return list;
}

int getAllUsersReportCount() {
  if (currentUser == NULL || !currentUser->isAdmin) {
    return 0;
  }

  int totalCount = 0;
  DIR *usersDir = opendir(USERS_DIR);
  if (usersDir == NULL) {
    return 0;
  }

  struct dirent *userEntry;
  char userReportsPath[1024];

  while ((userEntry = readdir(usersDir)) != NULL) {
    if (strcmp(userEntry->d_name, ".") == 0 ||
        strcmp(userEntry->d_name, "..") == 0) {
      continue;
    }

    snprintf(userReportsPath, sizeof(userReportsPath), "%s/%s", USERS_DIR,
             userEntry->d_name);
    struct stat statbuf;
    if (stat(userReportsPath, &statbuf) != 0 || !S_ISDIR(statbuf.st_mode)) {
      continue;
    }

    snprintf(userReportsPath, sizeof(userReportsPath), "%s/%s/reports",
             USERS_DIR, userEntry->d_name);
    DIR *reportsDir = opendir(userReportsPath);
    if (reportsDir == NULL) {
      continue;
    }

    struct dirent *reportEntry;
    while ((reportEntry = readdir(reportsDir)) != NULL) {
      if (strcmp(reportEntry->d_name, ".") == 0 ||
          strcmp(reportEntry->d_name, "..") == 0) {
        continue;
      }

      if (strstr(reportEntry->d_name, "report_") == reportEntry->d_name &&
          strstr(reportEntry->d_name, ".txt") != NULL) {
        totalCount++;
      }
    }

    closedir(reportsDir);
  }

  closedir(usersDir);
  return totalCount;
}

struct MonthReport *findReportInListByDate(struct MonthReportList *list,
                                           time_t date) {
  if (list == NULL || list->reports == NULL) {
    return NULL;
  }

  struct tm *targetTime = localtime(&date);
  int targetYear = targetTime->tm_year + 1900;
  int targetMonth = targetTime->tm_mon + 1;

  for (int i = 0; i < list->count; i++) {
    if (list->reports[i] != NULL) {
      struct tm *reportTime = localtime(&list->reports[i]->date);
      int reportYear = reportTime->tm_year + 1900;
      int reportMonth = reportTime->tm_mon + 1;

      if (reportYear == targetYear && reportMonth == targetMonth) {
        return list->reports[i];
      }
    }
  }

  return NULL;
}

void sortReportListByDate(struct MonthReportList *list, int ascending) {
  if (list == NULL || list->reports == NULL || list->count <= 1) {
    return;
  }

  for (int i = 0; i < list->count - 1; i++) {
    for (int j = 0; j < list->count - i - 1; j++) {
      if (list->reports[j] != NULL && list->reports[j + 1] != NULL) {
        int shouldSwap =
            ascending ? (list->reports[j]->date > list->reports[j + 1]->date)
                      : (list->reports[j]->date < list->reports[j + 1]->date);

        if (shouldSwap) {
          struct MonthReport *temp = list->reports[j];
          list->reports[j] = list->reports[j + 1];
          list->reports[j + 1] = temp;
        }
      }
    }
  }
}

struct MonthReportList *filterReportsByDateRange(struct MonthReportList *list,
                                                 time_t startDate,
                                                 time_t endDate) {
  if (list == NULL) {
    return NULL;
  }

  struct MonthReportList *filteredList = createMonthReportList();
  if (filteredList == NULL) {
    return NULL;
  }

  for (int i = 0; i < list->count; i++) {
    if (list->reports[i] != NULL) {
      time_t reportDate = list->reports[i]->date;
      if (reportDate >= startDate && reportDate <= endDate) {

        addMonthReportToList(filteredList, list->reports[i]);
      }
    }
  }

  return filteredList;
}

#endif
