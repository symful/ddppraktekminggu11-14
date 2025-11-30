#include "../auth/auth.h"
#include "../types/include.h"
#include "../utils/env_loader.c"
#include "./month_report_list.c"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

#ifndef DB_USER_MANAGEMENT_C
#define DB_USER_MANAGEMENT_C

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

  struct CategoryList *categories = initializeDefaultCategories();
  if (categories != NULL) {
    saveUserCategories(categories);
    freeCategoryList(categories);
  }

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

void resetUserBudgets(struct MonthReportList *monthReportList) {
  if (monthReportList == NULL || currentUser == NULL || currentUser->isAdmin) {
    return;
  }

  struct CategoryList *categories = getUserCategoriesCache();
  if (categories == NULL) {
    return;
  }

  for (int i = 0; i < monthReportList->count; i++) {
    struct MonthReport *report = monthReportList->reports[i];

    for (int j = 0; j < categories->count; j++) {
      if (categories->items[j] != NULL) {
        setBudgetForCategory(report, categories->items[j]->internalName,
                             1000000);
      }
    }

    saveUserMonthReport(report);
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

  clearScreen();
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

    struct MonthReportList *reportList = listUserMonthReports();
    if (reportList == NULL || reportList->count == 0) {
      printf("   📋 Tidak ada laporan ditemukan.\n\n");
      if (reportList)
        free(reportList);
      continue;
    }

    printf("   📋 Total Laporan: %d\n", reportList->count);

    long long totalIncome = 0, totalExpenses = 0;
    for (int j = 0; j < reportList->count; j++) {
      struct MonthReport *report = reportList->reports[j];
      totalIncome += report->totalIncome;
      totalExpenses += report->totalExpenses;

      struct tm *timeinfo = localtime(&report->date);
      printf(
          "   📅 %04d-%02d: Pemasukan: %lld, Pengeluaran: %lld, Saldo: %lld\n",
          timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, report->totalIncome,
          report->totalExpenses, report->balance);
    }

    printf("   💰 Ringkasan Total - Pemasukan: %lld, Pengeluaran: %lld, "
           "Bersih: %lld\n\n",
           totalIncome, totalExpenses, totalIncome - totalExpenses);

    if (reportList->reports) {
      for (int j = 0; j < reportList->count; j++) {
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

  clearScreen();
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
      struct MonthReportList *reportList = listUserMonthReports();
      if (reportList != NULL && reportList->count > 0) {
        totalReports += reportList->count;

        for (int j = 0; j < reportList->count; j++) {
          struct MonthReport *report = reportList->reports[j];
          systemTotalIncome += report->totalIncome;
          systemTotalExpense += report->totalExpenses;
        }

        if (reportList->reports) {
          for (int j = 0; j < reportList->count; j++) {
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

void runAdminDiagnostics() {
  printf("╔══════════════════════════════════════════════════════════╗\n");
  printf("║               🔍 ADMIN SYSTEM DIAGNOSTICS               ║\n");
  printf("╚══════════════════════════════════════════════════════════╝\n\n");

  printf("1. Memeriksa direktori users...\n");
  struct stat st = {0};
  if (stat(USERS_DIR, &st) == 0 && S_ISDIR(st.st_mode)) {
    printf("   ✅ Direktori users ada: %s\n", USERS_DIR);
  } else {
    printf("   ❌ Direktori users tidak ditemukan: %s\n", USERS_DIR);
    return;
  }

  printf("\n2. Memeriksa status admin pengguna...\n");
  struct UserList *userList = getAllUsers();
  if (userList == NULL) {
    printf("   ❌ Gagal mengambil daftar pengguna\n");
    return;
  }

  if (userList->count == 0) {
    printf("   ⚠️ Tidak ada pengguna ditemukan\n");
  } else {
    printf("   📊 Total pengguna: %d\n", userList->count);
    int adminCount = 0;

    char *adminUsername = envConfig.adminUsername;
    for (int i = 0; i < userList->count; i++) {
      int isAdmin =
          (adminUsername && strcmp(userList->usernames[i], adminUsername) == 0);
      printf("   👤 %s: %s\n", userList->usernames[i],
             isAdmin ? "ADMIN ✅" : "USER");
      if (isAdmin) {
        adminCount++;
      }

      char userPath[256];
      snprintf(userPath, sizeof(userPath), "%s/%s", USERS_DIR,
               userList->usernames[i]);

      if (stat(userPath, &st) == 0 && S_ISDIR(st.st_mode)) {
        printf("      📂 Direktori: ✅\n");

        char reportsPath[256];
        snprintf(reportsPath, sizeof(reportsPath), "%s/%s/reports", USERS_DIR,
                 userList->usernames[i]);

        if (stat(reportsPath, &st) == 0 && S_ISDIR(st.st_mode)) {
          printf("      📁 Reports: ✅\n");
        } else {
          printf("      📁 Reports: ❌\n");
        }

        char hashPath[256];
        snprintf(hashPath, sizeof(hashPath), "%s/%s/password.hash", USERS_DIR,
                 userList->usernames[i]);

        if (stat(hashPath, &st) == 0) {
          printf("      🔒 Password: ✅\n");
        } else {
          printf("      🔒 Password: ❌\n");
        }
      } else {
        printf("      📂 Direktori: ❌\n");
      }
    }

    printf("   👑 Total admin: %d\n", adminCount);

    if (adminCount == 0) {
      printf("   ⚠️ PERINGATAN: Admin hanya ada di file .env!\n");
    }
  }

  printf("\n3. Memeriksa pengguna saat ini...\n");
  if (currentUser == NULL) {
    printf("   ❌ Tidak ada pengguna yang sedang login\n");
  } else {
    printf("   👤 Pengguna aktif: %s\n", currentUser->username);
    printf("   🔐 Status admin: %s\n",
           currentUser->isAdmin ? "YA ✅" : "TIDAK");
    printf("   📁 Direktori user: %s\n", currentUser->userDir);

    if (stat(currentUser->userDir, &st) == 0 && S_ISDIR(st.st_mode)) {
      printf("   📂 Direktori user valid: ✅\n");
    } else {
      printf("   📂 Direktori user tidak valid: ❌\n");
    }
  }

  printf("\n4. Memeriksa integritas file admin...\n");
  if (userList->count > 0) {
    char *adminUsername = envConfig.adminUsername;
    printf("   📄 Admin didefinisikan di .env: %s\n",
           adminUsername ? adminUsername : "TIDAK DISET");

    if (adminUsername) {
      int adminExists = 0;
      for (int i = 0; i < userList->count; i++) {
        if (strcmp(userList->usernames[i], adminUsername) == 0) {
          adminExists = 1;
          printf("   👑 Admin '%s' terdaftar sebagai user ✅\n", adminUsername);
          break;
        }
      }
      if (!adminExists) {
        printf("   ⚠️ Admin '%s' belum terdaftar sebagai user\n", adminUsername);
      }
    }
  }

  printf("\n5. Memeriksa direktori reports global...\n");
  if (stat("./reports", &st) == 0 && S_ISDIR(st.st_mode)) {
    printf("   ✅ Direktori reports global ada\n");
  } else {
    printf("   ❌ Direktori reports global tidak ditemukan\n");
  }

  freeUserList(userList);

  printf("\n╔══════════════════════════════════════════════════════════╗\n");
  printf("║                    DIAGNOSIS SELESAI                    ║\n");
  printf("╚══════════════════════════════════════════════════════════╝\n");
  printf("\nTekan Enter untuk melanjutkan...");
  getchar();
}

void fixAdminSystem() {
  printf("╔══════════════════════════════════════════════════════════╗\n");
  printf("║                 🔧 PERBAIKAN ADMIN SYSTEM               ║\n");
  printf("╚══════════════════════════════════════════════════════════╝\n\n");

  struct stat st = {0};
  if (stat(USERS_DIR, &st) == -1) {
    printf("📁 Membuat direktori users...\n");
    if (mkdir(USERS_DIR, 0700) == 0) {
      printf("   ✅ Direktori users berhasil dibuat\n");
    } else {
      printf("   ❌ Gagal membuat direktori users\n");
      return;
    }
  }

  if (stat("./reports", &st) == -1) {
    printf("📁 Membuat direktori reports global...\n");
    if (mkdir("./reports", 0700) == 0) {
      printf("   ✅ Direktori reports global berhasil dibuat\n");
    } else {
      printf("   ❌ Gagal membuat direktori reports global\n");
    }
  }

  printf("\n🔍 Memeriksa konfigurasi admin di .env...\n");
  char *adminUsername = envConfig.adminUsername;
  char *adminPassword = envConfig.adminPassword;

  if (!adminUsername || !adminPassword) {
    printf("❌ Admin tidak dikonfigurasi di file .env!\n");
    printf("   Tambahkan ADMIN_USERNAME dan ADMIN_PASSWORD di file .env\n");
    return;
  }

  printf("✅ Admin dikonfigurasi: %s\n", adminUsername);

  if (userExists(adminUsername)) {
    printf("✅ Admin '%s' sudah terdaftar sebagai user\n", adminUsername);
  } else {
    printf("ℹ️ Admin '%s' belum terdaftar sebagai user (ini normal)\n",
           adminUsername);
    printf("   Admin dapat login langsung menggunakan kredensial .env\n");
  }

  printf("\n🔍 Memeriksa dan memperbaiki pengguna yang rusak...\n");
  struct UserList *userList = getAllUsers();
  if (userList != NULL) {
    for (int i = 0; i < userList->count; i++) {
      char *username = userList->usernames[i];
      printf("   🔧 Memperbaiki pengguna: %s\n", username);

      if (!createUserDirectory(username)) {
        printf("      ❌ Gagal membuat direktori user\n");
        continue;
      }

      if (!createUserReportsDirectory(username)) {
        printf("      ❌ Gagal membuat direktori reports user\n");
        continue;
      }

      printf("      ✅ Pengguna diperbaiki\n");
    }
    freeUserList(userList);
  }

  printf("\n✅ Perbaikan admin system selesai!\n");
  printf("Tekan Enter untuk melanjutkan...");
  getchar();
}

void showEnhancedAdminMenu() {
  printf("╔══════════════════════════════════════════════════════════╗\n");
  printf("║                   👑 PANEL ADMIN ENHANCED               ║\n");
  printf("╠══════════════════════════════════════════════════════════╣\n");
  printf("║  1. 👥 Kelola Pengguna                                  ║\n");
  printf("║  2. 📊 Lihat Semua Laporan Pengguna                    ║\n");
  printf("║  3. 📈 Statistik Sistem                                 ║\n");
  printf("║  4. 🔍 Diagnostik Admin System                          ║\n");
  printf("║  5. 🔧 Perbaiki Admin System                            ║\n");
  printf("║  6. 🔙 Lanjutkan sebagai Pengguna Biasa                 ║\n");
  printf("║  7. 🔓 Logout                                           ║\n");
  printf("╚══════════════════════════════════════════════════════════╝\n");
  printf("Masukkan pilihan Anda (1-7): ");
}

int handleEnhancedAdminMenu() {
  while (1) {
    clearScreen();
    showEnhancedAdminMenu();

    int choice;
    if (scanf("%d", &choice) != 1) {
      int c;
      while ((c = getchar()) != '\n' && c != EOF)
        ;
      printf("❌ Input tidak valid!\n");
      printf("Tekan Enter untuk melanjutkan...");
      getchar();
      continue;
    }

    int c;
    while ((c = getchar()) != '\n' && c != EOF)
      ;

    switch (choice) {
    case 1:
      return handleAdminUserManagement();
    case 2:
      viewAllUserReports();
      printf("Tekan Enter untuk melanjutkan...");
      getchar();
      break;
    case 3:
      showSystemStatistics();
      printf("Tekan Enter untuk melanjutkan...");
      getchar();
      break;
    case 4:
      runAdminDiagnostics();
      break;
    case 5:
      fixAdminSystem();
      break;
    case 6:
      return 1;
    case 7:
      return 0;
    default:
      printf("❌ Pilihan tidak valid!\n");
      printf("Tekan Enter untuk melanjutkan...");
      getchar();
      break;
    }
  }
}

int validateAdminPrivileges(const char *operation) {
  if (currentUser == NULL) {
    printf("❌ Tidak ada pengguna yang login!\n");
    return 0;
  }

  if (!currentUser->isAdmin) {
    printf("❌ Akses ditolak: Operasi '%s' memerlukan hak admin!\n", operation);
    return 0;
  }

  printf("✅ Hak admin terverifikasi untuk operasi: %s\n", operation);
  return 1;
}

int forcePasswordReset(const char *targetUsername) {
  if (!validateAdminPrivileges("force password reset")) {
    return 0;
  }

  if (!userExists(targetUsername)) {
    printf("❌ Pengguna '%s' tidak ditemukan!\n", targetUsername);
    return 0;
  }

  char newPassword[256];
  printf("🔐 Reset password untuk pengguna: %s\n", targetUsername);

  printf("Password baru: ");
  if (scanf("%255s", newPassword) == 1) {
    if (changeUserPassword(targetUsername, newPassword)) {
      printf("✅ Password berhasil direset untuk '%s'!\n", targetUsername);
      return 1;
    } else {
      printf("❌ Gagal mereset password!\n");
    }
  }

  return 0;
}

void showAdminAccessSummary() {
  if (!currentUser || !currentUser->isAdmin) {
    return;
  }

  printf("\n╔══════════════════════════════════════════════════════════╗\n");
  printf("║                  👑 STATUS ADMIN AKTIF                  ║\n");
  printf("╠══════════════════════════════════════════════════════════╣\n");
  printf("║  Admin: %-47s ║\n", currentUser->username);
  printf("║  Direktori: %-43s ║\n", currentUser->userDir);
  printf("║                                                          ║\n");
  printf("║  🔑 Akses Penuh ke Semua Fitur Sistem                   ║\n");
  printf("║  👥 Kelola Semua Pengguna                               ║\n");
  printf("║  📊 Lihat Laporan Semua Pengguna                       ║\n");
  printf("║  🔧 Diagnostik dan Perbaikan Sistem                    ║\n");
  printf("╚══════════════════════════════════════════════════════════╝\n\n");
}

#endif
