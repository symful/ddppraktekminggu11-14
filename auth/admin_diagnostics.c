#include "../utils/env_loader.c"
#include "../utils/secure_input.c"
#include "auth.h"
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

#ifndef ADMIN_DIAGNOSTICS_C
#define ADMIN_DIAGNOSTICS_C

// Diagnostic function to check admin system integrity
void runAdminDiagnostics() {
  printf("╔══════════════════════════════════════════════════════════╗\n");
  printf("║               🔍 ADMIN SYSTEM DIAGNOSTICS               ║\n");
  printf("╚══════════════════════════════════════════════════════════╝\n\n");

  // Check 1: Users directory exists
  printf("1. Memeriksa direktori users...\n");
  struct stat st = {0};
  if (stat(USERS_DIR, &st) == 0 && S_ISDIR(st.st_mode)) {
    printf("   ✅ Direktori users ada: %s\n", USERS_DIR);
  } else {
    printf("   ❌ Direktori users tidak ditemukan: %s\n", USERS_DIR);
    return;
  }

  // Check 2: List all users and their admin status
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
    }

    printf("   👑 Total admin: %d\n", adminCount);

    if (adminCount == 0) {
      printf("   ⚠️ PERINGATAN: Admin hanya ada di file .env!\n");
    }
  }

  // Check 3: Current user status
  printf("\n3. Memeriksa pengguna saat ini...\n");
  if (currentUser == NULL) {
    printf("   ❌ Tidak ada pengguna yang sedang login\n");
  } else {
    printf("   👤 Pengguna aktif: %s\n", currentUser->username);
    printf("   🔐 Status admin: %s\n",
           currentUser->isAdmin ? "YA ✅" : "TIDAK");
    printf("   📁 Direktori user: %s\n", currentUser->userDir);

    // Check if user directory exists
    if (stat(currentUser->userDir, &st) == 0 && S_ISDIR(st.st_mode)) {
      printf("   📂 Direktori user valid: ✅\n");
    } else {
      printf("   📂 Direktori user tidak valid: ❌\n");
    }
  }

  // Check 4: Admin file integrity
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

  freeUserList(userList);

  printf("\n╔══════════════════════════════════════════════════════════╗\n");
  printf("║                    DIAGNOSIS SELESAI                    ║\n");
  printf("╚══════════════════════════════════════════════════════════╝\n");
}

// Fix admin system issues
void fixAdminSystem() {
  printf("╔══════════════════════════════════════════════════════════╗\n");
  printf("║                 🔧 PERBAIKAN ADMIN SYSTEM               ║\n");
  printf("╚══════════════════════════════════════════════════════════╝\n\n");

  // Ensure users directory exists
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

  // Check for admin users
  printf("🔍 Memeriksa konfigurasi admin di .env...\n");
  char *adminUsername = envConfig.adminUsername;
  char *adminPassword = envConfig.adminPassword;

  if (!adminUsername || !adminPassword) {
    printf("❌ Admin tidak dikonfigurasi di file .env!\n");
    printf("   Tambahkan ADMIN_USERNAME dan ADMIN_PASSWORD di file .env\n");
    return;
  }

  printf("✅ Admin dikonfigurasi: %s\n", adminUsername);

  // Check if admin user exists as regular user
  if (userExists(adminUsername)) {
    printf("✅ Admin '%s' sudah terdaftar sebagai user\n", adminUsername);
  } else {
    printf("ℹ️ Admin '%s' belum terdaftar sebagai user (ini normal)\n",
           adminUsername);
    printf("   Admin dapat login langsung menggunakan kredensial .env\n");
  }

  printf("\n✅ Perbaikan admin system selesai!\n");
}

// Enhanced admin menu with diagnostics
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

// Handle enhanced admin menu choice
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
      printf("Tekan Enter untuk melanjutkan...");
      getchar();
      break;
    case 5:
      fixAdminSystem();
      printf("Tekan Enter untuk melanjutkan...");
      getchar();
      break;
    case 6:
      return 1; // Continue as regular user
    case 7:
      return 0; // Logout
    default:
      printf("❌ Pilihan tidak valid!\n");
      printf("Tekan Enter untuk melanjutkan...");
      getchar();
      break;
    }
  }
}

// Validate admin privileges before sensitive operations
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

// Admin-only function to force password reset for any user
int forcePasswordReset(const char *targetUsername) {
  if (!validateAdminPrivileges("force password reset")) {
    return 0;
  }

  if (!userExists(targetUsername)) {
    printf("❌ Pengguna '%s' tidak ditemukan!\n", targetUsername);
    return 0;
  }

  char newPassword[MAX_PASSWORD_LENGTH];
  printf("🔐 Reset password untuk pengguna: %s\n", targetUsername);

  if (readSecurePasswordWithStrengthCheck("Password baru: ", newPassword,
                                          sizeof(newPassword))) {
    if (changeUserPassword(targetUsername, newPassword)) {
      printf("✅ Password berhasil direset untuk '%s'!\n", targetUsername);
      clearPassword(newPassword, sizeof(newPassword));
      return 1;
    } else {
      printf("❌ Gagal mereset password!\n");
    }
  }

  clearPassword(newPassword, sizeof(newPassword));
  return 0;
}

// Show admin access summary
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

#endif // ADMIN_DIAGNOSTICS_C
