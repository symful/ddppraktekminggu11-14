#include "../auth/auth.h"
#include "../db/month_report_list.c"
#include "../types/include.h"
#include "../ui/auth_ui.h"
#include "../ui/colors.c"
#include "../ui/master.h"
#include "../ui/screen.c"
#include "../utils/env_loader.c"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

#ifndef UI_ADMIN_MENU_UI_C
#define UI_ADMIN_MENU_UI_C

// Forward declarations
void viewAllUserReports();
void showSystemStatistics();
void runAdminDiagnostics();
void fixAdminSystem();

void showEnhancedAdminMenu() {
  int boxWidth = 59;
  clearAndCenterVertically(14);

  printCenteredColored(
      "╔══════════════════════════════════════════════════════════╗\n",
      boxWidth, COLOR_BRIGHT_CYAN);
  printCentered("║", boxWidth);
  printWithBg("                   👑 PANEL ADMIN ENHANCED               ",
              COLOR_BRIGHT_WHITE, BG_BLUE);
  printf("║\n");
  printCenteredColored(
      "╠══════════════════════════════════════════════════════════╣\n",
      boxWidth, COLOR_BRIGHT_CYAN);
  printCentered("║  ", boxWidth);
  printColored("1.", COLOR_BRIGHT_CYAN);
  printf(" 👥 Kelola Pengguna                                  ║\n");
  printCentered("║  ", boxWidth);
  printColored("2.", COLOR_BRIGHT_CYAN);
  printf(" 📊 Lihat Semua Laporan Pengguna                    ║\n");
  printCentered("║  ", boxWidth);
  printColored("3.", COLOR_BRIGHT_CYAN);
  printf(" 📈 Statistik Sistem                                 ║\n");
  printCentered("║  ", boxWidth);
  printColored("4.", COLOR_BRIGHT_CYAN);
  printf(" 🔍 Diagnostik Admin System                          ║\n");
  printCentered("║  ", boxWidth);
  printColored("5.", COLOR_BRIGHT_CYAN);
  printf(" 🔧 Perbaiki Admin System                            ║\n");
  printCentered("║  ", boxWidth);
  printColored("6.", COLOR_BRIGHT_CYAN);
  printf(" 🔙 Lanjutkan sebagai Pengguna Biasa                 ║\n");
  printCentered("║  ", boxWidth);
  printColored("7.", COLOR_BRIGHT_CYAN);
  printf(" 🔓 Logout                                           ║\n");
  printCenteredColored(
      "╚══════════════════════════════════════════════════════════╝\n",
      boxWidth, COLOR_BRIGHT_CYAN);
  printf("\n");
  printCentered("", boxWidth);
  printColored("🎯 Masukkan pilihan Anda (1-7): ", COLOR_BRIGHT_YELLOW);
}

int handleEnhancedAdminMenu() {
  int boxWidth = 59;
  while (1) {
    clearScreen();
    showEnhancedAdminMenu();

    int choice;
    if (scanf("%d", &choice) != 1) {
      int c;
      while ((c = getchar()) != '\n' && c != EOF)
        ;
      printCentered("", boxWidth);
      printError("❌ Input tidak valid!\n");
      printCentered("", boxWidth);
      printInfo("Tekan Enter untuk melanjutkan...");
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
      break;
    case 3:
      showSystemStatistics();
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
      printCentered("", boxWidth);
      printError("❌ Pilihan tidak valid!\n");
      printCentered("", boxWidth);
      printInfo("Tekan Enter untuk melanjutkan...");
      getchar();
      break;
    }
  }
}

void showAdminAccessSummary() {
  if (!currentUser || !currentUser->isAdmin) {
    return;
  }

  int boxWidth = 59;
  printf("\n");
  printCenteredColored(
      "╔══════════════════════════════════════════════════════════╗\n",
      boxWidth, COLOR_BRIGHT_CYAN);
  printCentered("║", boxWidth);
  printWithBg("                  👑 STATUS ADMIN AKTIF                  ",
              COLOR_BRIGHT_WHITE, BG_BLUE);
  printf("║\n");
  printCenteredColored(
      "╠══════════════════════════════════════════════════════════╣\n",
      boxWidth, COLOR_BRIGHT_CYAN);
  printCentered("║  ", boxWidth);
  printColored("Admin: ", COLOR_BRIGHT_CYAN);
  printf("%-43s ║\n", currentUser->username);
  printCentered("║  ", boxWidth);
  printColored("Direktori: ", COLOR_BRIGHT_CYAN);
  printf("%-39s ║\n", currentUser->userDir);
  printCenteredColored(
      "╠══════════════════════════════════════════════════════════╣\n",
      boxWidth, COLOR_BRIGHT_CYAN);
  printCentered("║  ", boxWidth);
  printf("🔑 Akses Penuh ke Semua Fitur Sistem                   ║\n");
  printCentered("║  ", boxWidth);
  printf("👥 Kelola Semua Pengguna                               ║\n");
  printCentered("║  ", boxWidth);
  printf("📊 Lihat Laporan Semua Pengguna                       ║\n");
  printCentered("║  ", boxWidth);
  printf("🔧 Diagnostik dan Perbaikan Sistem                    ║\n");
  printCenteredColored(
      "╚══════════════════════════════════════════════════════════╝\n",
      boxWidth, COLOR_BRIGHT_CYAN);
  printf("\n");
}

void viewAllUserReports() {
  int boxWidth = 59;
  if (currentUser == NULL || !currentUser->isAdmin) {
    printCentered("", boxWidth);
    printError("Akses ditolak: Diperlukan hak istimewa admin.\n");
    return;
  }

  struct UserList *userList = getAllUsers();
  if (userList == NULL || userList->count == 0) {
    printCentered("", boxWidth);
    printWarning("Tidak ada pengguna ditemukan.\n");
    return;
  }

  clearScreen();
  printCenteredColored(
      "╔══════════════════════════════════════════════════════════╗\n",
      boxWidth, COLOR_BRIGHT_CYAN);
  printCentered("║", boxWidth);
  printWithBg("                📊 SEMUA LAPORAN PENGGUNA               ",
              COLOR_BRIGHT_WHITE, BG_BLUE);
  printf("║\n");
  printCenteredColored(
      "╚══════════════════════════════════════════════════════════╝\n",
      boxWidth, COLOR_BRIGHT_CYAN);
  printf("\n");

  for (int i = 0; i < userList->count; i++) {
    char *username = userList->usernames[i];
    char userReportsPath[256];
    snprintf(userReportsPath, sizeof(userReportsPath), "%s/%s/reports",
             USERS_DIR, username);

    char *adminUsername = envConfig.adminUsername;
    int isAdmin = (adminUsername && strcmp(username, adminUsername) == 0);
    printCentered("", boxWidth);
    printColored("👤 Pengguna: ", COLOR_BRIGHT_CYAN);
    printf("%s", username);
    if (isAdmin) {
      printf(" %s[ADMIN]%s", COLOR_BRIGHT_YELLOW, COLOR_RESET);
    } else {
      printf(" %s[USER]%s", COLOR_BRIGHT_GREEN, COLOR_RESET);
    }
    printf("\n");
    printCenteredColored(
        "────────────────────────────────────────────────────────\n", boxWidth,
        COLOR_BRIGHT_CYAN);

    DIR *userReportsDir = opendir(userReportsPath);
    if (userReportsDir == NULL) {
      printCentered("   ", boxWidth);
      printWarning("📁 Direktori laporan tidak ditemukan.\n\n");
      continue;
    }

    struct MonthReportList *reportList = listUserMonthReports();
    if (reportList == NULL || reportList->count == 0) {
      printCentered("   ", boxWidth);
      printWarning("📋 Tidak ada laporan ditemukan.\n\n");
      if (reportList)
        free(reportList);
      continue;
    }

    printCentered("   ", boxWidth);
    printColored("📋 Total Laporan: ", COLOR_BRIGHT_CYAN);
    printf("%d\n", reportList->count);

    long long totalIncome = 0, totalExpenses = 0;
    for (int j = 0; j < reportList->count; j++) {
      struct MonthReport *report = reportList->reports[j];
      totalIncome += report->totalIncome;
      totalExpenses += report->totalExpenses;

      struct tm *timeinfo = localtime(&report->date);
      printCentered("   ", boxWidth);
      printf("📅 %s%04d-%02d%s: ", COLOR_BRIGHT_CYAN, timeinfo->tm_year + 1900,
             timeinfo->tm_mon + 1, COLOR_RESET);
      printf("%sPemasukan:%s %s%lld%s, ", COLOR_BRIGHT_GREEN, COLOR_RESET,
             COLOR_BRIGHT_GREEN, report->totalIncome, COLOR_RESET);
      printf("%sPengeluaran:%s %s%lld%s, ", COLOR_BRIGHT_RED, COLOR_RESET,
             COLOR_BRIGHT_RED, report->totalExpenses, COLOR_RESET);
      printf("%sSaldo:%s %s%lld%s\n", COLOR_BRIGHT_YELLOW, COLOR_RESET,
             COLOR_BRIGHT_YELLOW, report->balance, COLOR_RESET);
    }

    printCentered("   ", boxWidth);
    printColored("💰 Ringkasan Total - ", COLOR_BRIGHT_CYAN);
    printf("%sPemasukan:%s %s%lld%s, ", COLOR_BRIGHT_GREEN, COLOR_RESET,
           COLOR_BRIGHT_GREEN, totalIncome, COLOR_RESET);
    printf("%sPengeluaran:%s %s%lld%s, ", COLOR_BRIGHT_RED, COLOR_RESET,
           COLOR_BRIGHT_RED, totalExpenses, COLOR_RESET);
    printf("%sBersih:%s %s%lld%s\n\n", COLOR_BRIGHT_YELLOW, COLOR_RESET,
           COLOR_BRIGHT_YELLOW, totalIncome - totalExpenses, COLOR_RESET);

    if (reportList->reports) {
      for (int j = 0; j < reportList->count; j++) {
        freeMonthReport(reportList->reports[j]);
      }
      free(reportList->reports);
    }
    free(reportList);
  }

  freeUserList(userList);
  printf("\n");
  printCentered("", boxWidth);
  printInfo("📱 Tekan Enter untuk melanjutkan...");
  getchar();
}

void showSystemStatistics() {
  int boxWidth = 59;
  if (currentUser == NULL || !currentUser->isAdmin) {
    printCentered("", boxWidth);
    printError("Akses ditolak: Diperlukan hak istimewa admin.\n");
    return;
  }

  struct UserList *userList = getAllUsers();
  if (userList == NULL) {
    printCentered("", boxWidth);
    printError("Error mengambil daftar pengguna.\n");
    return;
  }

  clearScreen();
  printCenteredColored(
      "╔══════════════════════════════════════════════════════════╗\n",
      boxWidth, COLOR_BRIGHT_CYAN);
  printCentered("║", boxWidth);
  printWithBg("                  📈 STATISTIK SISTEM                   ",
              COLOR_BRIGHT_WHITE, BG_BLUE);
  printf("║\n");
  printCenteredColored(
      "╚══════════════════════════════════════════════════════════╝\n",
      boxWidth, COLOR_BRIGHT_CYAN);
  printf("\n");

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

  printCentered("", boxWidth);
  printColored("👥 Total Pengguna: ", COLOR_BRIGHT_CYAN);
  printf("%s%d%s\n", COLOR_BRIGHT_YELLOW, totalUsers, COLOR_RESET);
  printCentered("", boxWidth);
  printColored("👑 Pengguna Admin: ", COLOR_BRIGHT_CYAN);
  printf("%s%d%s\n", COLOR_BRIGHT_YELLOW, adminUsers, COLOR_RESET);
  printCentered("", boxWidth);
  printColored("👤 Pengguna Biasa: ", COLOR_BRIGHT_CYAN);
  printf("%s%d%s\n", COLOR_BRIGHT_YELLOW, totalUsers - adminUsers, COLOR_RESET);
  printCentered("", boxWidth);
  printColored("📊 Total Laporan: ", COLOR_BRIGHT_CYAN);
  printf("%s%d%s\n", COLOR_BRIGHT_YELLOW, totalReports, COLOR_RESET);
  printCentered("", boxWidth);
  printColored("💰 Total Pemasukan Sistem: ", COLOR_BRIGHT_GREEN);
  printf("%s%lld%s\n", COLOR_BRIGHT_GREEN, systemTotalIncome, COLOR_RESET);
  printCentered("", boxWidth);
  printColored("💸 Total Pengeluaran Sistem: ", COLOR_BRIGHT_RED);
  printf("%s%lld%s\n", COLOR_BRIGHT_RED, systemTotalExpense, COLOR_RESET);
  printCentered("", boxWidth);
  printColored("📈 Saldo Bersih Sistem: ", COLOR_BRIGHT_YELLOW);
  printf("%s%lld%s\n", COLOR_BRIGHT_YELLOW,
         systemTotalIncome - systemTotalExpense, COLOR_RESET);

  freeUserList(userList);
  printf("\n");
  printCentered("", boxWidth);
  printInfo("📱 Tekan Enter untuk melanjutkan...");
  getchar();
}

void runAdminDiagnostics() {
  int boxWidth = 59;
  printCenteredColored(
      "╔══════════════════════════════════════════════════════════╗\n",
      boxWidth, COLOR_BRIGHT_CYAN);
  printCentered("║", boxWidth);
  printWithBg("               🔍 ADMIN SYSTEM DIAGNOSTICS               ",
              COLOR_BRIGHT_WHITE, BG_BLUE);
  printf("║\n");
  printCenteredColored(
      "╚══════════════════════════════════════════════════════════╝\n",
      boxWidth, COLOR_BRIGHT_CYAN);
  printf("\n");

  printCentered("", boxWidth);
  printColored("1. Memeriksa direktori users...\n", COLOR_BRIGHT_CYAN);
  struct stat st = {0};
  if (stat(USERS_DIR, &st) == 0 && S_ISDIR(st.st_mode)) {
    printCentered("   ", boxWidth);
    printSuccess("✅ Direktori users ada: ");
    printf("%s\n", USERS_DIR);
  } else {
    printCentered("   ", boxWidth);
    printError("❌ Direktori users tidak ditemukan: ");
    printf("%s\n", USERS_DIR);
    return;
  }

  printf("\n");
  printCentered("", boxWidth);
  printColored("2. Memeriksa status admin pengguna...\n", COLOR_BRIGHT_CYAN);
  struct UserList *userList = getAllUsers();
  if (userList == NULL) {
    printCentered("   ", boxWidth);
    printError("❌ Gagal mengambil daftar pengguna\n");
    return;
  }

  if (userList->count == 0) {
    printCentered("   ", boxWidth);
    printWarning("⚠️ Tidak ada pengguna ditemukan\n");
  } else {
    printCentered("   ", boxWidth);
    printInfo("📊 Total pengguna: ");
    printf("%d\n", userList->count);
    int adminCount = 0;

    char *adminUsername = envConfig.adminUsername;
    for (int i = 0; i < userList->count; i++) {
      int isAdmin =
          (adminUsername && strcmp(userList->usernames[i], adminUsername) == 0);
      printCentered("   ", boxWidth);
      printf("👤 %s: ", userList->usernames[i]);
      if (isAdmin) {
        printSuccess("ADMIN ✅\n");
      } else {
        printInfo("USER\n");
      }
      if (isAdmin) {
        adminCount++;
      }

      char userPath[256];
      snprintf(userPath, sizeof(userPath), "%s/%s", USERS_DIR,
               userList->usernames[i]);

      if (stat(userPath, &st) == 0 && S_ISDIR(st.st_mode)) {
        printCentered("      ", boxWidth);
        printSuccess("📂 Direktori: ✅\n");

        char reportsPath[256];
        snprintf(reportsPath, sizeof(reportsPath), "%s/%s/reports", USERS_DIR,
                 userList->usernames[i]);

        if (stat(reportsPath, &st) == 0 && S_ISDIR(st.st_mode)) {
          printCentered("      ", boxWidth);
          printSuccess("📁 Reports: ✅\n");
        } else {
          printCentered("      ", boxWidth);
          printError("📁 Reports: ❌\n");
        }

        char hashPath[256];
        snprintf(hashPath, sizeof(hashPath), "%s/%s/password.hash", USERS_DIR,
                 userList->usernames[i]);

        if (stat(hashPath, &st) == 0) {
          printCentered("      ", boxWidth);
          printSuccess("🔒 Password: ✅\n");
        } else {
          printCentered("      ", boxWidth);
          printError("🔒 Password: ❌\n");
        }
      } else {
        printCentered("      ", boxWidth);
        printError("📂 Direktori: ❌\n");
      }
    }

    printCentered("   ", boxWidth);
    printInfo("👑 Total admin: ");
    printf("%d\n", adminCount);

    if (adminCount == 0) {
      printCentered("   ", boxWidth);
      printWarning("⚠️ PERINGATAN: Admin hanya ada di file .env!\n");
    }
  }

  printf("\n");
  printCentered("", boxWidth);
  printColored("3. Memeriksa pengguna saat ini...\n", COLOR_BRIGHT_CYAN);
  if (currentUser == NULL) {
    printCentered("   ", boxWidth);
    printError("❌ Tidak ada pengguna yang sedang login\n");
  } else {
    printCentered("   ", boxWidth);
    printInfo("👤 Pengguna aktif: ");
    printf("%s\n", currentUser->username);
    printCentered("   ", boxWidth);
    printInfo("🔐 Status admin: ");
    printf("%s\n", currentUser->isAdmin ? "YA ✅" : "TIDAK");
    printCentered("   ", boxWidth);
    printInfo("📁 Direktori user: ");
    printf("%s\n", currentUser->userDir);

    if (stat(currentUser->userDir, &st) == 0 && S_ISDIR(st.st_mode)) {
      printCentered("   ", boxWidth);
      printSuccess("📂 Direktori user valid: ✅\n");
    } else {
      printCentered("   ", boxWidth);
      printError("📂 Direktori user tidak valid: ❌\n");
    }
  }

  printf("\n");
  printCentered("", boxWidth);
  printColored("4. Memeriksa integritas file admin...\n", COLOR_BRIGHT_CYAN);
  if (userList->count > 0) {
    char *adminUsername = envConfig.adminUsername;
    printCentered("   ", boxWidth);
    printInfo("📄 Admin didefinisikan di .env: ");
    printf("%s\n", adminUsername ? adminUsername : "TIDAK DISET");

    if (adminUsername) {
      int adminExists = 0;
      for (int i = 0; i < userList->count; i++) {
        if (strcmp(userList->usernames[i], adminUsername) == 0) {
          adminExists = 1;
          printCentered("   ", boxWidth);
          printSuccess("👑 Admin '");
          printf("%s", adminUsername);
          printSuccess("' terdaftar sebagai user ✅\n");
          break;
        }
      }
      if (!adminExists) {
        printCentered("   ", boxWidth);
        printWarning("⚠️ Admin '");
        printf("%s", adminUsername);
        printWarning("' belum terdaftar sebagai user\n");
      }
    }
  }

  printf("\n");
  printCentered("", boxWidth);
  printColored("5. Memeriksa direktori reports global...\n", COLOR_BRIGHT_CYAN);
  if (stat("./reports", &st) == 0 && S_ISDIR(st.st_mode)) {
    printCentered("   ", boxWidth);
    printSuccess("✅ Direktori reports global ada\n");
  } else {
    printCentered("   ", boxWidth);
    printError("❌ Direktori reports global tidak ditemukan\n");
  }

  freeUserList(userList);

  printf("\n");
  printCenteredColored(
      "╔══════════════════════════════════════════════════════════╗\n",
      boxWidth, COLOR_BRIGHT_CYAN);
  printCentered("║", boxWidth);
  printWithBg("                    DIAGNOSIS SELESAI                    ",
              COLOR_BRIGHT_WHITE, BG_GREEN);
  printf("║\n");
  printCenteredColored(
      "╚══════════════════════════════════════════════════════════╝\n",
      boxWidth, COLOR_BRIGHT_CYAN);
  printf("\n");
  printCentered("", boxWidth);
  printInfo("📱 Tekan Enter untuk melanjutkan...");
  getchar();
}

void fixAdminSystem() {
  int boxWidth = 59;
  printCenteredColored(
      "╔══════════════════════════════════════════════════════════╗\n",
      boxWidth, COLOR_BRIGHT_CYAN);
  printCentered("║", boxWidth);
  printWithBg("                 🔧 PERBAIKAN ADMIN SYSTEM               ",
              COLOR_BRIGHT_WHITE, BG_BLUE);
  printf("║\n");
  printCenteredColored(
      "╚══════════════════════════════════════════════════════════╝\n",
      boxWidth, COLOR_BRIGHT_CYAN);
  printf("\n");

  struct stat st = {0};
  if (stat(USERS_DIR, &st) == -1) {
    printCentered("", boxWidth);
    printInfo("📁 Membuat direktori users...\n");
    if (mkdir(USERS_DIR, 0700) == 0) {
      printCentered("   ", boxWidth);
      printSuccess("✅ Direktori users berhasil dibuat\n");
    } else {
      printCentered("   ", boxWidth);
      printError("❌ Gagal membuat direktori users\n");
      return;
    }
  }

  if (stat("./reports", &st) == -1) {
    printCentered("", boxWidth);
    printInfo("📁 Membuat direktori reports global...\n");
    if (mkdir("./reports", 0700) == 0) {
      printCentered("   ", boxWidth);
      printSuccess("✅ Direktori reports global berhasil dibuat\n");
    } else {
      printCentered("   ", boxWidth);
      printError("❌ Gagal membuat direktori reports global\n");
    }
  }

  printf("\n");
  printCentered("", boxWidth);
  printInfo("🔍 Memeriksa konfigurasi admin di .env...\n");
  char *adminUsername = envConfig.adminUsername;
  char *adminPassword = envConfig.adminPassword;

  if (!adminUsername || !adminPassword) {
    printCentered("", boxWidth);
    printError("❌ Admin tidak dikonfigurasi di file .env!\n");
    printCentered("   ", boxWidth);
    printf("Tambahkan ADMIN_USERNAME dan ADMIN_PASSWORD di file .env\n");
    return;
  }

  printCentered("", boxWidth);
  printSuccess("✅ Admin dikonfigurasi: ");
  printf("%s\n", adminUsername);

  if (userExists(adminUsername)) {
    printCentered("", boxWidth);
    printSuccess("✅ Admin '");
    printf("%s", adminUsername);
    printSuccess("' sudah terdaftar sebagai user\n");
  } else {
    printCentered("", boxWidth);
    printInfo("ℹ️ Admin '");
    printf("%s", adminUsername);
    printInfo("' belum terdaftar sebagai user (ini normal)\n");
    printCentered("   ", boxWidth);
    printf("Admin dapat login langsung menggunakan kredensial .env\n");
  }

  printf("\n");
  printCentered("", boxWidth);
  printInfo("🔍 Memeriksa dan memperbaiki pengguna yang rusak...\n");
  struct UserList *userList = getAllUsers();
  if (userList != NULL) {
    for (int i = 0; i < userList->count; i++) {
      char *username = userList->usernames[i];
      printCentered("   ", boxWidth);
      printInfo("🔧 Memperbaiki pengguna: ");
      printf("%s\n", username);

      if (!createUserDirectory(username)) {
        printCentered("      ", boxWidth);
        printError("❌ Gagal membuat direktori user\n");
        continue;
      }

      if (!createUserReportsDirectory(username)) {
        printCentered("      ", boxWidth);
        printError("❌ Gagal membuat direktori reports user\n");
        continue;
      }

      printCentered("      ", boxWidth);
      printSuccess("✅ Pengguna diperbaiki\n");
    }
    freeUserList(userList);
  }

  printf("\n");
  printCentered("", boxWidth);
  printSuccess("✅ Perbaikan admin system selesai!\n");
  printCentered("", boxWidth);
  printInfo("📱 Tekan Enter untuk melanjutkan...");
  getchar();
}

#endif
