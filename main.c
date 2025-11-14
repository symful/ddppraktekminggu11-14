#include "auth/admin_auth.c"
#include "auth/session_manager.c"
#include "utils/input_utils.c"
#include <stdio.h>

// Function prototypes
int handleMainMenu();
int handleAuthMenu();
int handleAdminPanel();
int handleUserManagement();

// Main function
int main() {
  // Initialize systems
  if (!initializeAdminAuth()) {
    showError("Gagal menginisialisasi sistem admin");
    return 1;
  }

  if (!initializeSessionManager()) {
    showError("Gagal menginisialisasi sistem sesi");
    return 1;
  }

  printf("╔══════════════════════════════════════════════════════════╗\n");
  printf("║                     💰 FOXIAN WALLET 💰                  ║\n");
  printf("║                     Sistem Terintegrasi                  ║\n");
  printf("╚══════════════════════════════════════════════════════════╝\n");
  printf("\nSistem berhasil diinisialisasi!\n");
  waitForEnter();

  // Main application loop
  while (1) {
    int result = handleAuthMenu();

    if (result == -1) {
      // Exit requested
      break;
    } else if (result == 1) {
      // User logged in successfully
      if (handleMainMenu() == -1) {
        break; // Exit application
      }
    } else if (result == 2) {
      // Admin logged in successfully
      if (handleAdminPanel() == -1) {
        break; // Exit application
      }
    }
    // Continue loop for result == 0 (failed login/register)
  }

  // Cleanup
  if (isUserLoggedIn()) {
    logoutUser();
  }
  if (isAdminLoggedIn()) {
    logoutAdmin();
  }

  printf("\n╔══════════════════════════════════════════════════════════╗\n");
  printf("║                    👋 TERIMA KASIH!                     ║\n");
  printf("║          Semoga keuangan Anda selalu terjaga! 💰        ║\n");
  printf("║                   Sampai jumpa lagi! 😊                 ║\n");
  printf("╚══════════════════════════════════════════════════════════╝\n");

  return 0;
}

// Handle authentication menu
int handleAuthMenu() {
  while (1) {
    clearScreen();
    showAuthMenu();

    int choice;
    if (!readIntegerRange("", 1, 4, &choice)) {
      showError("Pilihan tidak valid");
      continue;
    }

    switch (choice) {
    case 1: {
      // User login
      clearScreen();
      if (showLoginForm()) {
        showSuccess("Login berhasil!");
        return 1;
      } else {
        showError("Login gagal");
      }
      break;
    }
    case 2: {
      // User register
      clearScreen();
      if (showRegisterForm()) {
        showSuccess("Registrasi berhasil!");
      } else {
        showError("Registrasi gagal");
      }
      break;
    }
    case 3: {
      // Admin login
      clearScreen();
      if (showAdminLoginForm()) {
        showSuccess("Login admin berhasil!");
        return 2;
      } else {
        showError("Login admin gagal");
      }
      break;
    }
    case 4: {
      // Exit
      if (readConfirmation("Apakah Anda yakin ingin keluar?") == 1) {
        return -1;
      }
      break;
    }
    }
  }
}

// Handle main user menu (placeholder)
int handleMainMenu() {
  printf("\n╔══════════════════════════════════════════════════════════╗\n");
  printf("║              Selamat Datang, %-22s     ║\n", getCurrentUsername());
  printf("║          Pelacak Keuangan Pribadi Anda 💰              ║\n");
  printf("╚══════════════════════════════════════════════════════════╝\n");

  waitForEnter();

  while (1) {
    clearScreen();
    printf("┌─────────────────────────────────────────────────────────┐\n");
    printf("│                    🏠 MENU UTAMA                        │\n");
    printf("│              👤 Pengguna: %-27s │\n", getCurrentUsername());
    printf("├─────────────────────────────────────────────────────────┤\n");
    printf("│  1. 📊 Kelola Laporan Bulanan                           │\n");
    printf("│  2. 📈 Lihat Ringkasan Keuangan                         │\n");
    printf("│  3. ⚙️  Pengaturan Budget                                │\n");
    printf("│  4. 🔧 Pengaturan Sistem                                │\n");
    printf("│  5. 🔓 Logout                                           │\n");
    printf("│  6. ❌ Keluar                                           │\n");
    printf("└─────────────────────────────────────────────────────────┘\n");
    printf("\n💡 Tip: Pilih nomor menu yang diinginkan\n");
    printf("🎯 Pilihan Anda: ");

    int choice;
    if (!readIntegerRange("", 1, 6, &choice)) {
      showError("Pilihan tidak valid");
      continue;
    }

    switch (choice) {
    case 1:
      showInfo("Fitur Kelola Laporan Bulanan akan segera hadir!");
      break;
    case 2:
      showInfo("Fitur Ringkasan Keuangan akan segera hadir!");
      break;
    case 3:
      showInfo("Fitur Pengaturan Budget akan segera hadir!");
      break;
    case 4:
      showInfo("Fitur Pengaturan Sistem akan segera hadir!");
      break;
    case 5: {
      // Logout
      if (readConfirmation("Apakah Anda yakin ingin logout?") == 1) {
        logoutUser();
        return 0;
      }
      break;
    }
    case 6: {
      // Exit
      if (readConfirmation("Apakah Anda yakin ingin keluar?") == 1) {
        logoutUser();
        return -1;
      }
      break;
    }
    }
  }
}

// Handle admin panel
int handleAdminPanel() {
  printf("\n╔══════════════════════════════════════════════════════════╗\n");
  printf("║              Selamat Datang Admin, %-17s     ║\n",
         getCurrentAdminUsername());
  printf("║              Akses Administratif Diberikan 👑          ║\n");
  printf("╚══════════════════════════════════════════════════════════╝\n");

  waitForEnter();

  while (1) {
    clearScreen();
    showAdminMenu();

    int choice;
    if (!readIntegerRange("", 1, 5, &choice)) {
      showError("Pilihan tidak valid");
      continue;
    }

    switch (choice) {
    case 1: {
      // User management
      if (handleUserManagement() == -1) {
        return -1;
      }
      break;
    }
    case 2:
      showInfo("Fitur Lihat Semua Laporan akan segera hadir!");
      break;
    case 3:
      showInfo("Fitur Statistik Sistem akan segera hadir!");
      break;
    case 4: {
      // Continue as regular user - need to login as user first
      if (readConfirmation("Lanjutkan sebagai pengguna biasa?") == 1) {
        logoutAdmin();
        printf("\nSilakan login sebagai pengguna biasa:\n");
        waitForEnter();
        return 0;
      }
      break;
    }
    case 5: {
      // Logout
      if (readConfirmation("Apakah Anda yakin ingin logout?") == 1) {
        logoutAdmin();
        return 0;
      }
      break;
    }
    }
  }
}

// Handle user management
int handleUserManagement() {
  while (1) {
    clearScreen();
    showUserManagementMenu();

    int choice;
    if (!readIntegerRange("", 1, 5, &choice)) {
      showError("Pilihan tidak valid");
      continue;
    }

    switch (choice) {
    case 1: {
      // List all users
      clearScreen();
      printf("╔══════════════════════════════════════════════════════════╗\n");
      printf("║                  👥 SEMUA PENGGUNA                      ║\n");
      printf(
          "╚══════════════════════════════════════════════════════════╝\n\n");

      struct UserList *userList = getAllUsers();
      if (!userList || userList->count == 0) {
        printf("Tidak ada pengguna ditemukan.\n");
      } else {
        printf("Daftar Pengguna:\n");
        printf("────────────────\n");
        for (int i = 0; i < userList->count; i++) {
          printf("%d. 👤 %s\n", i + 1, userList->usernames[i]);
        }
      }

      if (userList) {
        freeUserList(userList);
      }
      waitForEnter();
      break;
    }
    case 2: {
      // Create new user
      clearScreen();
      printf("╔══════════════════════════════════════════════════════════╗\n");
      printf("║                  ➕ BUAT PENGGUNA BARU                  ║\n");
      printf("╚══════════════════════════════════════════════════════════╝\n");

      char username[MAX_USERNAME_LENGTH];
      char password[MAX_PASSWORD_LENGTH];

      if (readString("Username baru: ", username, sizeof(username)) &&
          readStringLength("Password baru: ", password, sizeof(password), 6)) {
        if (createUser(username, password)) {
          showSuccess("Pengguna berhasil dibuat!");
        } else {
          showError("Gagal membuat pengguna");
        }
      } else {
        showError("Input tidak valid");
      }
      break;
    }
    case 3: {
      // Reset password
      clearScreen();
      printf("╔══════════════════════════════════════════════════════════╗\n");
      printf("║                🔑 RESET PASSWORD PENGGUNA              ║\n");
      printf("╚══════════════════════════════════════════════════════════╝\n");

      char username[MAX_USERNAME_LENGTH];
      char newPassword[MAX_PASSWORD_LENGTH];

      if (readString("Username: ", username, sizeof(username)) &&
          readStringLength("Password baru: ", newPassword, sizeof(newPassword),
                           6)) {
        if (changeUserPassword(username, newPassword)) {
          showSuccess("Password berhasil diubah!");
        } else {
          showError("Gagal mengubah password");
        }
      } else {
        showError("Input tidak valid");
      }
      break;
    }
    case 4: {
      // Delete user
      clearScreen();
      printf("╔══════════════════════════════════════════════════════════╗\n");
      printf("║                   🗑️ HAPUS PENGGUNA                    ║\n");
      printf("╚══════════════════════════════════════════════════════════╝\n");

      char username[MAX_USERNAME_LENGTH];
      if (readString("Username yang akan dihapus: ", username,
                     sizeof(username))) {
        if (readConfirmation(
                "Apakah Anda yakin ingin menghapus pengguna ini?") == 1) {
          if (deleteUser(username)) {
            showSuccess("Pengguna berhasil dihapus!");
          } else {
            showError("Gagal menghapus pengguna");
          }
        } else {
          showInfo("Penghapusan dibatalkan");
        }
      } else {
        showError("Input tidak valid");
      }
      break;
    }
    case 5: {
      // Back to admin menu
      return 0;
    }
    }
  }
}
