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

// Generate user-specific report filename
char *generateUserReportFilename(time_t date) {
  if (currentUser == NULL || currentUser->isAdmin) {
    return NULL; // Admin users don't have personal reports
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

// Save month report to user-specific file
void saveUserMonthReportToFile(struct MonthReport *report) {
  if (report == NULL || currentUser == NULL || currentUser->isAdmin) {
    return; // Admin users don't save personal reports
  }

  char *filename = generateUserReportFilename(report->date);
  if (filename != NULL) {
    saveMonthReport(report, filename);
    free(filename);
  }
}

// Delete user-specific month report file
void deleteUserMonthReportFile(struct MonthReport *report) {
  if (report == NULL || currentUser == NULL || currentUser->isAdmin) {
    return; // Admin users don't have personal reports to delete
  }

  char *filename = generateUserReportFilename(report->date);
  if (filename != NULL) {
    remove(filename);
    free(filename);
  }
}

// Load user-specific month report
struct MonthReport *loadUserMonthReport(time_t date) {
  if (currentUser == NULL || currentUser->isAdmin) {
    return NULL; // Admin users don't have personal reports
  }

  char *filename = generateUserReportFilename(date);
  if (filename == NULL) {
    return NULL;
  }

  struct MonthReport *report = loadMonthReport(filename);
  free(filename);
  return report;
}

// Check if user report exists for a specific date
int userReportExists(time_t date) {
  if (currentUser == NULL || currentUser->isAdmin) {
    return 0; // Admin users don't have personal reports
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

// Get user's reports directory path
char *getUserReportsDirectory() {
  if (currentUser == NULL || currentUser->isAdmin) {
    return NULL; // Admin users don't have personal report directories
  }

  static char reportsDir[256];
  snprintf(reportsDir, sizeof(reportsDir), "%s/%s/reports", USERS_DIR,
           currentUser->username);
  return reportsDir;
}

// Get user's config file path
char *getUserConfigPath() {
  if (currentUser == NULL || currentUser->isAdmin) {
    return NULL; // Admin users use system config, not personal config
  }

  static char configPath[256];
  snprintf(configPath, sizeof(configPath), "%s/%s/config.txt", USERS_DIR,
           currentUser->username);
  return configPath;
}

// Create default config for new user
void createDefaultUserConfig() {
  if (currentUser == NULL || currentUser->isAdmin) {
    return; // Admin users don't need personal configs
  }

  char *configPath = getUserConfigPath();
  if (configPath == NULL) {
    return;
  }

  // Check if config already exists
  FILE *file = fopen(configPath, "r");
  if (file != NULL) {
    fclose(file);
    return; // Config already exists
  }

  // Create default config
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

// Initialize user workspace (create directories and default files)
int initializeUserWorkspace(const char *username) {
  // Create user directory
  if (!createUserDirectory(username)) {
    return 0;
  }

  // Create reports directory
  if (!createUserReportsDirectory(username)) {
    return 0;
  }

  // Set current user temporarily to create config
  struct User *tempUser = createUserSession(username);
  if (tempUser == NULL) {
    return 0;
  }

  struct User *oldUser = currentUser;
  currentUser = tempUser;

  // Create default config
  createDefaultUserConfig();

  // Restore previous user
  currentUser = oldUser;
  free(tempUser);

  return 1;
}

// Clean up user session and save any pending data
void cleanupUserSession() {
  if (currentUser != NULL) {
    // Save any pending configuration (only for regular users)
    if (!currentUser->isAdmin) {
      char *configPath = getUserConfigPath();
      if (configPath != NULL) {
        saveConfigToFile(configPath);
      }
    }

    // Destroy session
    destroyUserSession();
  }
}

// Get list of all users (for admin purposes) - using function from auth.c

// User-aware wrappers for existing report functions
// These functions override the default behavior to use user-specific paths

// Wrapper for setBudgetForCategory that saves to user directory
void setUserCategoryBudget(struct MonthReport *report,
                           enum TransactionCategory category,
                           long long budget) {
  if (report == NULL || currentUser == NULL || currentUser->isAdmin) {
    return; // Admin users can't set personal budgets
  }

  setBudgetForCategory(report, category, budget);
  saveUserMonthReportToFile(report);
}

// Wrapper for addMonthReportTransaction that saves to user directory
void addUserMonthReportTransaction(struct MonthReport *monthReport,
                                   struct Transaction *transaction) {
  if (monthReport == NULL || transaction == NULL || currentUser == NULL ||
      currentUser->isAdmin) {
    return; // Admin users can't add personal transactions
  }

  addMonthReportTransaction(monthReport, transaction);
  saveUserMonthReportToFile(monthReport);
}

// Wrapper for removeMonthReportTransaction that saves to user directory
void removeUserMonthReportTransaction(struct MonthReport *monthReport,
                                      int groupIndex, int transactionIndex) {
  if (monthReport == NULL || currentUser == NULL || currentUser->isAdmin) {
    return; // Admin users can't remove personal transactions
  }

  removeMonthReportTransaction(monthReport, groupIndex, transactionIndex);
  saveUserMonthReportToFile(monthReport);
}

// Wrapper for updating report date that saves to user directory
void updateUserReportDate(struct MonthReport *report, time_t newDate) {
  if (report == NULL || currentUser == NULL || currentUser->isAdmin) {
    return; // Admin users can't update personal reports
  }

  deleteUserMonthReportFile(report);
  report->date = newDate;
  saveUserMonthReportToFile(report);
}

// Reset all budgets for user
void resetUserBudgets(struct MonthReportList *monthReportList) {
  if (monthReportList == NULL || currentUser == NULL || currentUser->isAdmin) {
    return; // Admin users can't reset personal budgets
  }

  for (int i = 0; i < monthReportList->amount; i++) {
    struct MonthReport *report = monthReportList->reports[i];

    for (int j = 0; j <= TC_OTHER; j++) {
      setBudgetForCategory(report, (enum TransactionCategory)j, 1000000);
    }

    saveUserMonthReportToFile(report);
  }
}

// Admin function to view all user reports
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

    // Free the report list
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

// Admin function to get system statistics
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

        // Free the report list
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

// Override existing functions to use user-specific paths when user is logged in
#ifdef USER_AWARE_REPORTS

// User-aware wrapper for saveMonthReportToFile
void saveUserAwareMonthReportToFile(struct MonthReport *report) {
  char *filename = generateUserReportFilename(report->date);
  saveMonthReport(report, filename);
  free(filename);
}

// User-aware wrapper for deleteMonthReportFile
void deleteUserAwareMonthReportFile(struct MonthReport *report) {
  char *filename = generateUserReportFilename(report->date);
  remove(filename);
  free(filename);
}

#endif

#endif
