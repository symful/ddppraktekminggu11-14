#include "../utils/env_loader.c"
#include "../utils/secure_input.c"
#include <stdio.h>
#include <string.h>

#ifndef ADMIN_AUTH_C
#define ADMIN_AUTH_C

// Initialize admin system
int initializeAdminAuth() { return loadEnvFile(); }

// Validate admin credentials
int validateAdminCredentials(const char *username, const char *password) {
  if (!username || !password) {
    return 0;
  }

  const char *validUsername = getAdminUsername();
  const char *validPassword = getAdminPassword();

  return (strcmp(username, validUsername) == 0 &&
          strcmp(password, validPassword) == 0);
}

// Show admin login form
int showAdminLoginForm() {
  char username[100];
  char password[100];

  printf("╔══════════════════════════════════════════════════════════╗\n");
  printf("║                    👑 MASUK ADMIN                       ║\n");
  printf("╚══════════════════════════════════════════════════════════╝\n");

  if (!readSecureUsername("Username Admin: ", username, sizeof(username))) {
    printf("❌ Error membaca username.\n");
    return 0;
  }

  if (!readSecurePassword("Password Admin: ", password, sizeof(password))) {
    printf("❌ Error membaca password.\n");
    clearPassword(password, sizeof(password));
    return 0;
  }

  int result = 0;
  if (validateAdminCredentials(username, password)) {
    printf("✅ Akses admin diberikan!\n");
    result = 1;
  } else {
    printf("❌ Username atau password admin salah.\n");
    result = 0;
  }

  // Clear password from memory after use
  clearPassword(password, sizeof(password));
  return result;
}

// Show admin menu
void showAdminMenu() {
  printf("╔══════════════════════════════════════════════════════════╗\n");
  printf("║                     👑 PANEL ADMIN                      ║\n");
  printf("╠══════════════════════════════════════════════════════════╣\n");
  printf("║  1. 👥 Kelola Pengguna                                  ║\n");
  printf("║  2. 📊 Lihat Semua Laporan Pengguna                    ║\n");
  printf("║  3. 📈 Statistik Sistem                                 ║\n");
  printf("║  4. 🔙 Lanjutkan sebagai Pengguna Biasa                 ║\n");
  printf("║  5. 🔓 Logout                                           ║\n");
  printf("╚══════════════════════════════════════════════════════════╝\n");
  printf("Masukkan pilihan Anda (1-5): ");
}

#endif
