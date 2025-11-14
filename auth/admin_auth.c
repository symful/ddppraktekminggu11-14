#include "../utils/env_loader.c"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef ADMIN_AUTH_C
#define ADMIN_AUTH_C

// Simple admin session structure
struct AdminSession {
  int isLoggedIn;
  char username[50];
};

static struct AdminSession adminSession = {0, ""};

// Initialize admin system
int initializeAdminAuth() { return loadEnvFile(); }

// Check if admin is currently logged in
int isAdminLoggedIn() { return adminSession.isLoggedIn; }

// Get current admin username
const char *getCurrentAdminUsername() {
  if (adminSession.isLoggedIn) {
    return adminSession.username;
  }
  return NULL;
}

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

// Login admin
int loginAdmin(const char *username, const char *password) {
  if (!validateAdminCredentials(username, password)) {
    return 0;
  }

  adminSession.isLoggedIn = 1;
  strncpy(adminSession.username, username, sizeof(adminSession.username) - 1);
  adminSession.username[sizeof(adminSession.username) - 1] = '\0';
  return 1;
}

// Logout admin
void logoutAdmin() {
  adminSession.isLoggedIn = 0;
  memset(adminSession.username, 0, sizeof(adminSession.username));
}

// Show admin login form
int showAdminLoginForm() {
  char username[100];
  char password[100];

  printf("╔══════════════════════════════════════════════════════════╗\n");
  printf("║                    👑 MASUK ADMIN                       ║\n");
  printf("╚══════════════════════════════════════════════════════════╝\n");

  printf("Username Admin: ");
  if (fgets(username, sizeof(username), stdin) == NULL) {
    printf("Error membaca username.\n");
    return 0;
  }

  // Remove newline
  size_t len = strlen(username);
  if (len > 0 && username[len - 1] == '\n') {
    username[len - 1] = '\0';
  }

  printf("Password Admin: ");
  if (fgets(password, sizeof(password), stdin) == NULL) {
    printf("Error membaca password.\n");
    return 0;
  }

  // Remove newline
  len = strlen(password);
  if (len > 0 && password[len - 1] == '\n') {
    password[len - 1] = '\0';
  }

  if (loginAdmin(username, password)) {
    printf("Akses admin diberikan!\n");
    return 1;
  } else {
    printf("Username atau password admin salah.\n");
    return 0;
  }
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
