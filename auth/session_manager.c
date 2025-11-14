#include "user_manager.c"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef SESSION_MANAGER_C
#define SESSION_MANAGER_C

// Session structure
struct UserSession {
  int isLoggedIn;
  char username[MAX_USERNAME_LENGTH];
  char userDir[256];
  char reportsDir[256];
};

static struct UserSession currentSession = {0, "", "", ""};

// Initialize session system
int initializeSessionManager() { return createUsersDirectory(); }

// Check if user is logged in
int isUserLoggedIn() { return currentSession.isLoggedIn; }

// Get current username
const char *getCurrentUsername() {
  if (currentSession.isLoggedIn) {
    return currentSession.username;
  }
  return NULL;
}

// Get current user directory
const char *getCurrentUserDir() {
  if (currentSession.isLoggedIn) {
    return currentSession.userDir;
  }
  return NULL;
}

// Get current user reports directory
const char *getCurrentUserReportsDir() {
  if (currentSession.isLoggedIn) {
    return currentSession.reportsDir;
  }
  return NULL;
}

// Login user
int loginUser(const char *username, const char *password) {
  if (!username || !password) {
    printf("Username dan password tidak boleh kosong.\n");
    return 0;
  }

  if (!authenticateUser(username, password)) {
    return 0;
  }

  // Create session
  currentSession.isLoggedIn = 1;
  strncpy(currentSession.username, username,
          sizeof(currentSession.username) - 1);
  currentSession.username[sizeof(currentSession.username) - 1] = '\0';

  snprintf(currentSession.userDir, sizeof(currentSession.userDir), "%s/%s",
           USERS_DIR, username);
  snprintf(currentSession.reportsDir, sizeof(currentSession.reportsDir),
           "%s/%s/reports", USERS_DIR, username);

  printf("Selamat datang kembali, %s!\n", username);
  return 1;
}

// Register new user
int registerUser(const char *username, const char *password) {
  if (!username || !password) {
    printf("Username dan password tidak boleh kosong.\n");
    return 0;
  }

  if (!createUser(username, password)) {
    return 0;
  }

  printf("Pengguna '%s' berhasil didaftarkan!\n", username);
  return 1;
}

// Logout user
void logoutUser() {
  if (currentSession.isLoggedIn) {
    printf("Sampai jumpa, %s!\n", currentSession.username);
    currentSession.isLoggedIn = 0;
    memset(currentSession.username, 0, sizeof(currentSession.username));
    memset(currentSession.userDir, 0, sizeof(currentSession.userDir));
    memset(currentSession.reportsDir, 0, sizeof(currentSession.reportsDir));
  }
}

// Show login form
int showLoginForm() {
  char username[MAX_USERNAME_LENGTH];
  char password[MAX_PASSWORD_LENGTH];

  printf("╔══════════════════════════════════════════════════════════╗\n");
  printf("║                       🔐 MASUK                          ║\n");
  printf("╚══════════════════════════════════════════════════════════╝\n");

  printf("Username: ");
  if (fgets(username, sizeof(username), stdin) == NULL) {
    printf("Error membaca username.\n");
    return 0;
  }

  // Remove newline
  size_t len = strlen(username);
  if (len > 0 && username[len - 1] == '\n') {
    username[len - 1] = '\0';
  }

  printf("Password: ");
  if (fgets(password, sizeof(password), stdin) == NULL) {
    printf("Error membaca password.\n");
    return 0;
  }

  // Remove newline
  len = strlen(password);
  if (len > 0 && password[len - 1] == '\n') {
    password[len - 1] = '\0';
  }

  return loginUser(username, password);
}

// Show register form
int showRegisterForm() {
  char username[MAX_USERNAME_LENGTH];
  char password[MAX_PASSWORD_LENGTH];
  char confirmPassword[MAX_PASSWORD_LENGTH];

  printf("╔══════════════════════════════════════════════════════════╗\n");
  printf("║                      📝 DAFTAR                          ║\n");
  printf("╚══════════════════════════════════════════════════════════╝\n");

  printf("Pilih username: ");
  if (fgets(username, sizeof(username), stdin) == NULL) {
    printf("Error membaca username.\n");
    return 0;
  }

  // Remove newline
  size_t len = strlen(username);
  if (len > 0 && username[len - 1] == '\n') {
    username[len - 1] = '\0';
  }

  printf("Pilih password: ");
  if (fgets(password, sizeof(password), stdin) == NULL) {
    printf("Error membaca password.\n");
    return 0;
  }

  // Remove newline
  len = strlen(password);
  if (len > 0 && password[len - 1] == '\n') {
    password[len - 1] = '\0';
  }

  printf("Konfirmasi password: ");
  if (fgets(confirmPassword, sizeof(confirmPassword), stdin) == NULL) {
    printf("Error membaca konfirmasi password.\n");
    return 0;
  }

  // Remove newline
  len = strlen(confirmPassword);
  if (len > 0 && confirmPassword[len - 1] == '\n') {
    confirmPassword[len - 1] = '\0';
  }

  // Check if passwords match
  if (strcmp(password, confirmPassword) != 0) {
    printf("Password tidak cocok.\n");
    return 0;
  }

  return registerUser(username, password);
}

// Clear screen function
void clearScreen() {
#ifdef _WIN32
  system("cls");
#else
  system("clear");
#endif
}

// Show main authentication menu
void showAuthMenu() {
  printf("╔══════════════════════════════════════════════════════════╗\n");
  printf("║                 💰 FOXIAN WALLET 💰                  ║\n");
  printf("║                   Sistem Autentikasi                    ║\n");
  printf("╠══════════════════════════════════════════════════════════╣\n");
  printf("║  1. 🔐 Masuk                                            ║\n");
  printf("║  2. 📝 Daftar                                           ║\n");
  printf("║  3. 👑 Masuk Admin                                      ║\n");
  printf("║  4. 🚪 Keluar                                           ║\n");
  printf("╚══════════════════════════════════════════════════════════╝\n");
  printf("Masukkan pilihan Anda (1-4): ");
}

#endif
