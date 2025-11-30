#ifndef UI_AUTH_UI_C
#define UI_AUTH_UI_C

#include "../auth/admin_auth.c"
#include "../auth/auth.h"
#include "../utils/input_handler.h"
#include "../utils/secure_input.c"
#include "./colors.c"
#include "./screen.c"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void clearScreen();

int handleUserDeletion();
int handlePasswordReset();
int handleAdminUserCreation();

void showAuthMenu() {
  int boxWidth = 59;
  int contentLines = 11;
  clearAndCenterVertically(contentLines);

  printCenteredColored(
      "╔══════════════════════════════════════════════════════════╗\n",
      boxWidth, COLOR_BRIGHT_CYAN);
  printCentered("║", boxWidth);
  printWithBg("                 💰 FOXIAN WALLET 💰                     ",
              COLOR_BRIGHT_WHITE, BG_BLUE);
  printf("║\n");
  printCentered("║", boxWidth);
  printColored("                   Sistem Autentikasi                    ",
               COLOR_BRIGHT_CYAN);
  printf("║\n");
  printCenteredColored(
      "╠══════════════════════════════════════════════════════════╣\n",
      boxWidth, COLOR_BRIGHT_CYAN);
  printCentered("║  ", boxWidth);
  printColored("1.", COLOR_BRIGHT_CYAN);
  printf(" 🔐 Masuk                                            ║\n");
  printCentered("║  ", boxWidth);
  printColored("2.", COLOR_BRIGHT_CYAN);
  printf(" 📝 Daftar                                           ║\n");
  printCentered("║  ", boxWidth);
  printColored("3.", COLOR_BRIGHT_CYAN);
  printf(" 👑 Masuk Admin                                      ║\n");
  printCentered("║  ", boxWidth);
  printColored("4.", COLOR_BRIGHT_CYAN);
  printf(" 🚪 Keluar                                           ║\n");
  printCenteredColored(
      "╚══════════════════════════════════════════════════════════╝\n",
      boxWidth, COLOR_BRIGHT_CYAN);
  printf("\n");
  printCentered("", boxWidth);
  printColored("🎯 Masukkan pilihan Anda (1-4): ", COLOR_BRIGHT_YELLOW);
}

void showAdminMenu() {
  int boxWidth = 59;
  int contentLines = 11;
  clearAndCenterVertically(contentLines);

  printCenteredColored(
      "╔══════════════════════════════════════════════════════════╗\n",
      boxWidth, COLOR_BRIGHT_CYAN);
  printCentered("║", boxWidth);
  printWithBg("                     👑 PANEL ADMIN                      ",
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
  printf(" 🔙 Lanjutkan sebagai Pengguna Biasa                 ║\n");
  printCentered("║  ", boxWidth);
  printColored("5.", COLOR_BRIGHT_CYAN);
  printf(" 🔓 Logout                                           ║\n");
  printCenteredColored(
      "╚══════════════════════════════════════════════════════════╝\n",
      boxWidth, COLOR_BRIGHT_CYAN);
  printf("\n");
  printCentered("", boxWidth);
  printColored("🎯 Masukkan pilihan Anda (1-5): ", COLOR_BRIGHT_YELLOW);
}

void showUserManagementMenu() {
  int boxWidth = 59;
  int contentLines = 12;
  clearAndCenterVertically(contentLines);

  printCenteredColored(
      "╔══════════════════════════════════════════════════════════╗\n",
      boxWidth, COLOR_BRIGHT_CYAN);
  printCentered("║", boxWidth);
  printWithBg("                  👥 KELOLA PENGGUNA                     ",
              COLOR_BRIGHT_WHITE, BG_BLUE);
  printf("║\n");
  printCenteredColored(
      "╠══════════════════════════════════════════════════════════╣\n",
      boxWidth, COLOR_BRIGHT_CYAN);
  printCentered("║  ", boxWidth);
  printColored("1.", COLOR_BRIGHT_CYAN);
  printf(" 📋 Daftar Semua Pengguna                            ║\n");
  printCentered("║  ", boxWidth);
  printColored("2.", COLOR_BRIGHT_CYAN);
  printf(" ➕ Buat Pengguna Baru                               ║\n");
  printCentered("║  ", boxWidth);
  printColored("3.", COLOR_BRIGHT_CYAN);
  printf(" 🔑 Reset Password Pengguna                          ║\n");
  printCentered("║  ", boxWidth);
  printColored("4.", COLOR_BRIGHT_CYAN);
  printf(" 🗑️  Hapus Pengguna                                  ║\n");
  printCentered("║  ", boxWidth);
  printColored("5.", COLOR_BRIGHT_CYAN);
  printf(" ⬅️  Kembali                                          ║\n");
  printCenteredColored(
      "╚══════════════════════════════════════════════════════════╝\n",
      boxWidth, COLOR_BRIGHT_CYAN);
  printf("\n");
  printCentered("", boxWidth);
  printInfo("💡 Admin hanya dapat dikelola melalui file .env\n");
  printCentered("", boxWidth);
  printColored("🎯 Masukkan pilihan Anda (1-5): ", COLOR_BRIGHT_YELLOW);
}

void showLoginMenu() {
  int boxWidth = 59;
  int contentLines = 3;
  clearAndCenterVertically(contentLines);

  printCenteredColored(
      "╔══════════════════════════════════════════════════════════╗\n",
      boxWidth, COLOR_BRIGHT_CYAN);
  printCentered("║", boxWidth);
  printWithBg("                       🔐 MASUK                          ",
              COLOR_BRIGHT_WHITE, BG_BLUE);
  printf("║\n");
  printCenteredColored(
      "╚══════════════════════════════════════════════════════════╝\n",
      boxWidth, COLOR_BRIGHT_CYAN);
}

void showRegisterMenu() {
  int boxWidth = 59;
  int contentLines = 3;
  clearAndCenterVertically(contentLines);

  printCenteredColored(
      "╔══════════════════════════════════════════════════════════╗\n",
      boxWidth, COLOR_BRIGHT_CYAN);
  printCentered("║", boxWidth);
  printWithBg("                      📝 DAFTAR                          ",
              COLOR_BRIGHT_WHITE, BG_BLUE);
  printf("║\n");
  printCenteredColored(
      "╚══════════════════════════════════════════════════════════╝\n",
      boxWidth, COLOR_BRIGHT_CYAN);
}

void showAdminLoginForm() {
  int boxWidth = 59;
  int contentLines = 3;
  clearAndCenterVertically(contentLines);

  printCenteredColored(
      "╔══════════════════════════════════════════════════════════╗\n",
      boxWidth, COLOR_BRIGHT_CYAN);
  printCentered("║", boxWidth);
  printWithBg("                    👑 MASUK ADMIN                       ",
              COLOR_BRIGHT_WHITE, BG_BLUE);
  printf("║\n");
  printCenteredColored(
      "╚══════════════════════════════════════════════════════════╝\n",
      boxWidth, COLOR_BRIGHT_CYAN);
}

int handleLogin() {
  char username[MAX_USERNAME_LENGTH];
  char password[MAX_PASSWORD_LENGTH];

  showLoginMenu();

  printf("\n");
  int boxWidth = 59;
  printCentered("", boxWidth);
  if (!readSecureUsername("👤 Username: ", username, sizeof(username))) {
    printf("\n");
    printCentered("", boxWidth);
    printError("❌ Error membaca username.\n");
    return 0;
  }

  printf("\n");
  printCentered("", boxWidth);
  if (!readSecurePassword("🔒 Password: ", password, sizeof(password))) {
    printf("\n");
    printCentered("", boxWidth);
    printError("❌ Error membaca password.\n");
    clearPassword(password, sizeof(password));
    return 0;
  }

  int result = loginUser(username, password);
  clearPassword(password, sizeof(password));

  printf("\n");
  printCentered("", boxWidth);
  if (result) {
    printSuccess("✅ Login berhasil! Selamat datang, ");
    printColored(username, COLOR_BRIGHT_CYAN);
    printf("!\n");
  } else {
    printError("❌ Username atau password salah.\n");
  }

  return result;
}

int handleRegister() {
  char username[MAX_USERNAME_LENGTH];
  char password[MAX_PASSWORD_LENGTH];
  char confirmPassword[MAX_PASSWORD_LENGTH];

  showRegisterMenu();

  printf("\n");
  int boxWidth = 59;
  printCentered("", boxWidth);
  if (!readSecureUsername("👤 Username baru: ", username, sizeof(username))) {
    printf("\n");
    printCentered("", boxWidth);
    printError("❌ Error membaca username.\n");
    return 0;
  }

  printf("\n");
  printCentered("", boxWidth);
  if (!readSecurePassword("🔒 Password baru: ", password, sizeof(password))) {
    printf("\n");
    printCentered("", boxWidth);
    printError("❌ Error membaca password.\n");
    clearPassword(password, sizeof(password));
    return 0;
  }

  printf("\n");
  printCentered("", boxWidth);
  if (!readSecurePassword("🔒 Konfirmasi password: ", confirmPassword,
                          sizeof(confirmPassword))) {
    printf("\n");
    printCentered("", boxWidth);
    printError("❌ Error membaca konfirmasi password.\n");
    clearPassword(password, sizeof(password));
    return 0;
  }

  if (strcmp(password, confirmPassword) != 0) {
    printf("\n");
    printCentered("", boxWidth);
    printError("❌ Password tidak cocok.\n");
    clearPassword(password, sizeof(password));
    clearPassword(confirmPassword, sizeof(confirmPassword));
    return 0;
  }

  int result = registerUser(username, password);
  clearPassword(password, sizeof(password));
  clearPassword(confirmPassword, sizeof(confirmPassword));

  printf("\n");
  printCentered("", boxWidth);
  if (result) {
    printSuccess("✅ Registrasi berhasil! Silakan login.\n");
  } else {
    printError("❌ Registrasi gagal. Username mungkin sudah digunakan.\n");
  }

  return result;
}

int handleAdminLogin() {
  char username[100];
  char password[100];

  showAdminLoginForm();

  printf("\n");
  int boxWidth = 59;
  printCentered("", boxWidth);
  if (!readSecureUsername("👑 Username Admin: ", username, sizeof(username))) {
    printf("\n");
    printCentered("", boxWidth);
    printError("❌ Error membaca username.\n");
    return 0;
  }

  printf("\n");
  printCentered("", boxWidth);
  if (!readSecurePassword("🔐 Password Admin: ", password, sizeof(password))) {
    printf("\n");
    printCentered("", boxWidth);
    printError("❌ Error membaca password.\n");
    clearPassword(password, sizeof(password));
    return 0;
  }

  int result = 0;
  if (validateAdminCredentials(username, password)) {
    printf("\n");
    printCentered("", boxWidth);
    printSuccess("✅ Akses admin diberikan!\n");
    currentUser = createAdminUserSession(username);
    result = 1;
  } else {
    printf("\n");
    printCentered("", boxWidth);
    printError("❌ Username atau password admin salah.\n");
    result = 0;
  }

  clearPassword(password, sizeof(password));
  return result;
}

int handleUserDeletion() {
  clearScreen();
  int boxWidth = 59;
  printCenteredColored(
      "╔══════════════════════════════════════════════════════════╗\n",
      boxWidth, COLOR_BRIGHT_CYAN);
  printCentered("║", boxWidth);
  printWithBg("                    🗑️  HAPUS PENGGUNA                   ",
              COLOR_BRIGHT_WHITE, BG_BLUE);
  printf("║\n");
  printCenteredColored(
      "╚══════════════════════════════════════════════════════════╝\n",
      boxWidth, COLOR_BRIGHT_CYAN);

  char username[MAX_USERNAME_LENGTH];
  printf("\n");
  printCentered("", boxWidth);
  if (!readSecureUsername("👤 Username yang akan dihapus: ", username,
                          sizeof(username))) {
    printf("\n");
    printCentered("", boxWidth);
    printError("❌ Error membaca username.\n");
    printCentered("", boxWidth);
    printInfo("📱 Tekan Enter untuk melanjutkan...");
    getchar();
    return 0;
  }

  printf("\n");
  printCentered("", boxWidth);
  printWarning("⚠️  Yakin ingin menghapus pengguna '");
  printf("%s", username);
  printf("'? (y/n): ");

  char confirm;
  scanf(" %c", &confirm);
  clearInputBuffer();

  if (confirm != 'y' && confirm != 'Y') {
    printf("\n");
    printCentered("", boxWidth);
    printInfo("ℹ️  Penghapusan dibatalkan.\n");
    printCentered("", boxWidth);
    printInfo("📱 Tekan Enter untuk melanjutkan...");
    getchar();
    return 0;
  }

  int result = deleteUser(username);
  printf("\n");
  printCentered("", boxWidth);
  if (result) {
    printSuccess("✅ Pengguna berhasil dihapus!\n");
  } else {
    printError("❌ Gagal menghapus pengguna.\n");
  }

  printCentered("", boxWidth);
  printInfo("📱 Tekan Enter untuk melanjutkan...");
  getchar();
  return result;
}

int handlePasswordReset() {
  clearScreen();
  int boxWidth = 59;
  printCenteredColored(
      "╔══════════════════════════════════════════════════════════╗\n",
      boxWidth, COLOR_BRIGHT_CYAN);
  printCentered("║", boxWidth);
  printWithBg("                  🔑 RESET PASSWORD                      ",
              COLOR_BRIGHT_WHITE, BG_BLUE);
  printf("║\n");
  printCenteredColored(
      "╚══════════════════════════════════════════════════════════╝\n",
      boxWidth, COLOR_BRIGHT_CYAN);

  char username[MAX_USERNAME_LENGTH];
  char newPassword[MAX_PASSWORD_LENGTH];

  printf("\n");
  printCentered("", boxWidth);
  if (!readSecureUsername("👤 Username: ", username, sizeof(username))) {
    printf("\n");
    printCentered("", boxWidth);
    printError("❌ Error membaca username.\n");
    printCentered("", boxWidth);
    printInfo("📱 Tekan Enter untuk melanjutkan...");
    getchar();
    return 0;
  }

  printf("\n");
  printCentered("", boxWidth);
  if (!readSecurePassword("🔒 Password baru: ", newPassword,
                          sizeof(newPassword))) {
    printf("\n");
    printCentered("", boxWidth);
    printError("❌ Error membaca password.\n");
    clearPassword(newPassword, sizeof(newPassword));
    printCentered("", boxWidth);
    printInfo("📱 Tekan Enter untuk melanjutkan...");
    getchar();
    return 0;
  }

  int result = changeUserPassword(username, newPassword);
  clearPassword(newPassword, sizeof(newPassword));

  printf("\n");
  printCentered("", boxWidth);
  if (result) {
    printSuccess("✅ Password berhasil direset!\n");
  } else {
    printError("❌ Gagal mereset password.\n");
  }

  printCentered("", boxWidth);
  printInfo("📱 Tekan Enter untuk melanjutkan...");
  getchar();
  return result;
}

int handleAdminUserCreation() {
  clearScreen();
  int boxWidth = 59;
  printCenteredColored(
      "╔══════════════════════════════════════════════════════════╗\n",
      boxWidth, COLOR_BRIGHT_CYAN);
  printCentered("║", boxWidth);
  printWithBg("                  ➕ BUAT PENGGUNA BARU                  ",
              COLOR_BRIGHT_WHITE, BG_BLUE);
  printf("║\n");
  printCenteredColored(
      "╚══════════════════════════════════════════════════════════╝\n",
      boxWidth, COLOR_BRIGHT_CYAN);

  char username[MAX_USERNAME_LENGTH];
  char password[MAX_PASSWORD_LENGTH];

  printf("\n");
  printCentered("", boxWidth);
  if (!readSecureUsername("👤 Username baru: ", username, sizeof(username))) {
    printf("\n");
    printCentered("", boxWidth);
    printError("❌ Error membaca username.\n");
    printCentered("", boxWidth);
    printInfo("📱 Tekan Enter untuk melanjutkan...");
    getchar();
    return 0;
  }

  printf("\n");
  printCentered("", boxWidth);
  if (!readSecurePassword("🔒 Password: ", password, sizeof(password))) {
    printf("\n");
    printCentered("", boxWidth);
    printError("❌ Error membaca password.\n");
    clearPassword(password, sizeof(password));
    printCentered("", boxWidth);
    printInfo("📱 Tekan Enter untuk melanjutkan...");
    getchar();
    return 0;
  }

  int result = registerUser(username, password);
  clearPassword(password, sizeof(password));

  printf("\n");
  printCentered("", boxWidth);
  if (result) {
    printSuccess("✅ Pengguna berhasil dibuat!\n");
  } else {
    printError("❌ Gagal membuat pengguna.\n");
  }

  printCentered("", boxWidth);
  printInfo("📱 Tekan Enter untuk melanjutkan...");
  getchar();
  return result;
}

int handleAdminUserManagement() {
  while (1) {
    showUserManagementMenu();

    int choice;
    scanf("%d", &choice);
    clearInputBuffer();

    switch (choice) {
    case 1: {
      clearScreen();
      int boxWidth = 59;
      printCenteredColored(
          "╔══════════════════════════════════════════════════════════╗\n",
          boxWidth, COLOR_BRIGHT_CYAN);
      printCentered("║", boxWidth);
      printWithBg("                  📋 DAFTAR PENGGUNA                     ",
                  COLOR_BRIGHT_WHITE, BG_BLUE);
      printf("║\n");
      printCenteredColored(
          "╚══════════════════════════════════════════════════════════╝\n",
          boxWidth, COLOR_BRIGHT_CYAN);

      struct UserList *users = getAllUsers();
      if (users && users->count > 0) {
        printf("\n");
        printCentered("", boxWidth);
        printSuccess("👥 Total Pengguna: ");
        printf("%s%d%s\n\n", COLOR_BRIGHT_CYAN, users->count, COLOR_RESET);
        for (int i = 0; i < users->count; i++) {
          printCentered("  ", boxWidth);
          printf("%d.", i + 1);
          printf(" 👤 %s", users->usernames[i]);
          if (strcmp(users->usernames[i], "admin") == 0) {
            printf(" ");
            printColored("(Admin)", COLOR_BRIGHT_YELLOW);
          }
          printf("\n");
        }
        freeUserList(users);
      } else {
        printf("\n");
        printCentered("", boxWidth);
        printWarning("⚠️  Tidak ada pengguna terdaftar.\n");
      }
      printf("\n");
      printCentered("", boxWidth);
      printInfo("📱 Tekan Enter untuk melanjutkan...");
      getchar();
      break;
    }
    case 2:
      handleAdminUserCreation();
      break;
    case 3:
      handlePasswordReset();
      break;
    case 4:
      handleUserDeletion();
      break;
    case 5:
      return 1;
    default:
      clearScreen();
      int boxWidth = 59;
      printCentered("", boxWidth);
      printError("❌ Pilihan tidak valid.\n");
      printCentered("", boxWidth);
      printInfo("📱 Tekan Enter untuk melanjutkan...");
      getchar();
      break;
    }
  }
}

int handleAuthMenuChoice() {
  int choice;
  scanf("%d", &choice);
  clearInputBuffer();

  switch (choice) {
  case 1:
    return handleLogin();
  case 2:
    return handleRegister();
  case 3:
    return handleAdminLogin();
  case 4:
    printf("\n");
    int boxWidth = 59;
    printCentered("", boxWidth);
    printSuccess("👋 Terima kasih telah menggunakan FoxianWallet!\n");
    exit(0);
  default:
    clearScreen();
    printCentered("", boxWidth);
    printError("❌ Pilihan tidak valid.\n");
    return 0;
  }
}

#endif
