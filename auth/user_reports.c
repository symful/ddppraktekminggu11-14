#include "../db/include.c"
#include "../utils/env_loader.c"
#include "auth.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

void saveUserMonthReportToFile(struct MonthReport *report) {
  if (report == NULL || currentUser == NULL || currentUser->isAdmin) {
    return;
  }

  char *filename = generateUserReportFilename(report->date);
  if (filename != NULL) {
    saveMonthReport(report, filename);
    free(filename);
  }
}

void deleteUserMonthReportFile(struct MonthReport *report) {
  if (report == NULL || currentUser == NULL || currentUser->isAdmin) {
    return;
  }

  char *filename = generateUserReportFilename(report->date);
  if (filename != NULL) {
    remove(filename);
    free(filename);
  }
}

struct MonthReport *loadUserMonthReport(time_t date) {
  if (currentUser == NULL || currentUser->isAdmin) {
    return NULL;
  }

  char *filename = generateUserReportFilename(date);
  if (filename == NULL) {
    return NULL;
  }

  struct MonthReport *report = loadMonthReport(filename);
  free(filename);
  return report;
}

int userReportExists(time_t date) {
  if (currentUser == NULL || currentUser->isAdmin) {
    return 0;
  }

  char *filename = generateUserReportFilename(date);
  if (filename == NULL) {
    return 0;
  }

  FILE *file = fopen(filename, "r");
  free(filename);

  if (file != NULL) {
    fclose(file);
    return 1;
  }

  return 0;
}

char *getUserReportsDirectory() {
  if (currentUser == NULL || currentUser->isAdmin) {
    return NULL;
  }

  static char reportsDir[256];
  snprintf(reportsDir, sizeof(reportsDir), "%s/%s/reports", USERS_DIR,
           currentUser->username);
  return reportsDir;
}

char *getUserConfigPath() {
  if (currentUser == NULL || currentUser->isAdmin) {
    return NULL;
  }

  static char configPath[256];
  snprintf(configPath, sizeof(configPath), "%s/%s/config.txt", USERS_DIR,
           currentUser->username);
  return configPath;
}

void createDefaultUserConfig() {
  if (currentUser == NULL || currentUser->isAdmin) {
    return;
  }

  char *configPath = getUserConfigPath();
  if (configPath == NULL) {
    return;
  }

  FILE *file = fopen(configPath, "r");
  if (file != NULL) {
    fclose(file);
    return;
  }

  file = fopen(configPath, "w");
  if (file != NULL) {
    fprintf(file, "# User Configuration File\n");
    fprintf(file, "# Generated automatically for user: %s\n",
            currentUser->username);
    fprintf(file, "MINIMUM_AMOUNT=1000\n");
    fprintf(file, "MINIMUM_BUDGET=10000\n");
    fprintf(file, "MINIMUM_TRANSACTION=500\n");
    fclose(file);
  }
}

int initializeUserWorkspace(const char *username) {

  if (!createUserDirectory(username)) {
    return 0;
  }

  if (!createUserReportsDirectory(username)) {
    return 0;
  }

  struct User *tempUser = createUserSession(username);
  if (tempUser == NULL) {
    return 0;
  }

  struct User *oldUser = currentUser;
  currentUser = tempUser;

  createDefaultUserConfig();

  currentUser = oldUser;
  free(tempUser);

  return 1;
}

void cleanupUserSession() {
  if (currentUser != NULL) {

    if (!currentUser->isAdmin) {
      char *configPath = getUserConfigPath();
      if (configPath != NULL) {
        saveConfigToFile(configPath);
      }
    }

    destroyUserSession();
  }
}

void setUserCategoryBudget(struct MonthReport *report,
                           enum TransactionCategory category,
                           long long budget) {
  if (report == NULL || currentUser == NULL || currentUser->isAdmin) {
    return;
  }

  setBudgetForCategory(report, category, budget);
  saveUserMonthReportToFile(report);
}

void addUserMonthReportTransaction(struct MonthReport *monthReport,
                                   struct Transaction *transaction) {
  if (monthReport == NULL || transaction == NULL || currentUser == NULL ||
      currentUser->isAdmin) {
    return;
  }

  addMonthReportTransaction(monthReport, transaction);
  saveUserMonthReportToFile(monthReport);
}

void removeUserMonthReportTransaction(struct MonthReport *monthReport,
                                      int groupIndex, int transactionIndex) {
  if (monthReport == NULL || currentUser == NULL || currentUser->isAdmin) {
    return;
  }

  removeMonthReportTransaction(monthReport, groupIndex, transactionIndex);
  saveUserMonthReportToFile(monthReport);
}

void updateUserReportDate(struct MonthReport *report, time_t newDate) {
  if (report == NULL || currentUser == NULL || currentUser->isAdmin) {
    return;
  }

  deleteUserMonthReportFile(report);
  report->date = newDate;
  saveUserMonthReportToFile(report);
}

void resetUserBudgets(struct MonthReportList *monthReportList) {
  if (monthReportList == NULL || currentUser == NULL || currentUser->isAdmin) {
    return;
  }

  for (int i = 0; i < monthReportList->amount; i++) {
    struct MonthReport *report = monthReportList->reports[i];

    for (int j = 0; j <= TC_OTHER; j++) {
      setBudgetForCategory(report, (enum TransactionCategory)j, 1000000);
    }

    saveUserMonthReportToFile(report);
  }
}

void viewAllUserReports() {
  if (currentUser == NULL || !currentUser->isAdmin) {
    printf("Akses ditolak: Diperlukan hak istimewa admin.\n");
    return;
  }

  struct UserList *userList = getAllUsers();
  if (userList == NULL || userList->count == 0) {
    printf("Tidak ada pengguna ditemukan.\n");
    return;
  }

  clearAuthScreen();
  printf("╔══════════════════════════════════════════════════════════╗\n");
  printf("║                📊 SEMUA LAPORAN PENGGUNA               ║\n");
  printf("╚══════════════════════════════════════════════════════════╝\n\n");

  for (int i = 0; i < userList->count; i++) {
    char *username = userList->usernames[i];
    char userReportsPath[256];
    snprintf(userReportsPath, sizeof(userReportsPath), "%s/%s/reports",
             USERS_DIR, username);

    char *adminUsername = envConfig.adminUsername;
    int isAdmin = (adminUsername && strcmp(username, adminUsername) == 0);
    printf("👤 Pengguna: %s %s\n", username, isAdmin ? "[ADMIN]" : "[USER]");
    printf("────────────────────────────────────────────────────────\n");

    DIR *userReportsDir = opendir(userReportsPath);
    if (userReportsDir == NULL) {
      printf("   📁 Direktori laporan tidak ditemukan.\n\n");
      continue;
    }

    struct MonthReportList *reportList =
        listUserMonthReports(userReportsDir, userReportsPath);
    if (reportList == NULL || reportList->amount == 0) {
      printf("   📋 Tidak ada laporan ditemukan.\n\n");
      if (reportList)
        free(reportList);
      continue;
    }

    printf("   📋 Total Laporan: %d\n", reportList->amount);

    long long totalIncome = 0, totalExpense = 0;
    for (int j = 0; j < reportList->amount; j++) {
      struct MonthReport *report = reportList->reports[j];
      totalIncome += report->totalIncome;
      totalExpense += report->totalExpense;

      struct tm *timeinfo = localtime(&report->date);
      printf(
          "   📅 %04d-%02d: Pemasukan: %lld, Pengeluaran: %lld, Saldo: %lld\n",
          timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, report->totalIncome,
          report->totalExpense, report->balance);
    }

    printf("   💰 Ringkasan Total - Pemasukan: %lld, Pengeluaran: %lld, "
           "Bersih: %lld\n\n",
           totalIncome, totalExpense, totalIncome - totalExpense);

    if (reportList->reports) {
      for (int j = 0; j < reportList->amount; j++) {
        freeMonthReport(reportList->reports[j]);
      }
      free(reportList->reports);
    }
    free(reportList);
  }

  freeUserList(userList);
  printf("Tekan Enter untuk melanjutkan...");
  getchar();
}

void showSystemStatistics() {
  if (currentUser == NULL || !currentUser->isAdmin) {
    printf("Akses ditolak: Diperlukan hak istimewa admin.\n");
    return;
  }

  struct UserList *userList = getAllUsers();
  if (userList == NULL) {
    printf("Error mengambil daftar pengguna.\n");
    return;
  }

  clearAuthScreen();
  printf("╔══════════════════════════════════════════════════════════╗\n");
  printf("║                  📈 STATISTIK SISTEM                   ║\n");
  printf("╚══════════════════════════════════════════════════════════╝\n\n");

  int totalUsers = userList->count;
  int adminUsers = 0;
  int totalReports = 0;
  long long systemTotalIncome = 0;
  long long systemTotalExpense = 0;

  for (int i = 0; i < userList->count; i++) {
    char *adminUsername = envConfig.adminUsername;
    if (adminUsername && strcmp(userList->usernames[i], adminUsername) == 0) {
      adminUsers++;
    }

    char userReportsPath[256];
    snprintf(userReportsPath, sizeof(userReportsPath), "%s/%s/reports",
             USERS_DIR, userList->usernames[i]);

    DIR *userReportsDir = opendir(userReportsPath);
    if (userReportsDir != NULL) {
      struct MonthReportList *reportList =
          listUserMonthReports(userReportsDir, userReportsPath);
      if (reportList != NULL && reportList->amount > 0) {
        totalReports += reportList->amount;

        for (int j = 0; j < reportList->amount; j++) {
          struct MonthReport *report = reportList->reports[j];
          systemTotalIncome += report->totalIncome;
          systemTotalExpense += report->totalExpense;
        }

        if (reportList->reports) {
          for (int j = 0; j < reportList->amount; j++) {
            freeMonthReport(reportList->reports[j]);
          }
          free(reportList->reports);
        }
        free(reportList);
      }
    }
  }

  printf("👥 Total Pengguna: %d\n", totalUsers);
  printf("👑 Pengguna Admin: %d\n", adminUsers);
  printf("👤 Pengguna Biasa: %d\n", totalUsers - adminUsers);
  printf("📊 Total Laporan: %d\n", totalReports);
  printf("💰 Total Pemasukan Sistem: %lld\n", systemTotalIncome);
  printf("💸 Total Pengeluaran Sistem: %lld\n", systemTotalExpense);
  printf("📈 Saldo Bersih Sistem: %lld\n",
         systemTotalIncome - systemTotalExpense);

  freeUserList(userList);
  printf("\nTekan Enter untuk melanjutkan...");
  getchar();
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
