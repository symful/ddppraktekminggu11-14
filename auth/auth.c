
#include "auth.h"
#include "../types/include.c"
#include "../utils/env_loader.c"
#include "../utils/secure_input.c"
#include <crypt.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

// Global current user
struct User *currentUser = NULL;

// Simple hash function
char *hashPassword(const char *password) {
  static char hash[HASH_LENGTH];
  const char *salt = "$6$rounds=5000$mysalt123456$";
  char *result = crypt(password, salt);

  if (result == NULL) {
    return NULL;
  }

  strncpy(hash, result, HASH_LENGTH - 1);
  hash[HASH_LENGTH - 1] = '\0';
  return hash;
}

// Initialize authentication system
int initializeAuthSystem() {
  struct stat st = {0};

  // Create users directory if it doesn't exist
  if (stat(USERS_DIR, &st) == -1) {
    if (mkdir(USERS_DIR, 0700) != 0) {
      perror("Error creating users directory");
      return 0;
    }
  }

  // Admin is defined in .env file, no need to create or check
  printf("✅ Sistem autentikasi diinisialisasi.\n");

  return 1;
}

// Check if user exists
int userExists(const char *username) {
  char userDir[256];
  snprintf(userDir, sizeof(userDir), "%s/%s", USERS_DIR, username);

  struct stat st = {0};
  return (stat(userDir, &st) == 0);
}

// Create user directory
int createUserDirectory(const char *username) {
  char userDir[256];
  snprintf(userDir, sizeof(userDir), "%s/%s", USERS_DIR, username);

  struct stat st = {0};
  if (stat(userDir, &st) == -1) {
    if (mkdir(userDir, 0700) != 0) {
      perror("Error creating user directory");
      return 0;
    }
  }

  return 1;
}

// Create user reports directory
int createUserReportsDirectory(const char *username) {
  char reportsDir[256];
  snprintf(reportsDir, sizeof(reportsDir), "%s/%s/reports", USERS_DIR,
           username);

  struct stat st = {0};
  if (stat(reportsDir, &st) == -1) {
    if (mkdir(reportsDir, 0700) != 0) {
      perror("Error creating user reports directory");
      return 0;
    }
  }

  return 1;
}

// Save password hash to file
int savePasswordHash(const char *username, const char *hash) {
  char hashPath[256];
  snprintf(hashPath, sizeof(hashPath), "%s/%s/%s", USERS_DIR, username,
           HASH_FILENAME);

  FILE *file = fopen(hashPath, "w");
  if (file == NULL) {
    perror("Error saving password hash");
    return 0;
  }

  fprintf(file, "%s", hash);
  fclose(file);
  return 1;
}

// Load password hash from file
char *loadPasswordHash(const char *username) {
  char hashPath[256];
  static char hash[HASH_LENGTH];

  snprintf(hashPath, sizeof(hashPath), "%s/%s/%s", USERS_DIR, username,
           HASH_FILENAME);

  FILE *file = fopen(hashPath, "r");
  if (file == NULL) {
    return NULL;
  }

  if (fgets(hash, HASH_LENGTH, file) != NULL) {
    // Remove newline if present
    size_t len = strlen(hash);
    if (len > 0 && hash[len - 1] == '\n') {
      hash[len - 1] = '\0';
    }
    fclose(file);
    return hash;
  }

  fclose(file);
  return NULL;
}

// Verify password against stored hash
int verifyPassword(const char *password, const char *storedHash) {
  char *hashedInput = hashPassword(password);
  if (hashedInput == NULL) {
    return 0;
  }

  return (strcmp(hashedInput, storedHash) == 0);
}

// Get user reports path
char *getUserReportsPath(const char *username) {
  static char reportsPath[256];
  snprintf(reportsPath, sizeof(reportsPath), "%s/%s/reports", USERS_DIR,
           username);
  return reportsPath;
}

// Create user session
struct User *createUserSession(const char *username) {
  struct User *user = (struct User *)malloc(sizeof(struct User));
  if (user == NULL) {
    return NULL;
  }

  strncpy(user->username, username, MAX_USERNAME_LENGTH - 1);
  user->username[MAX_USERNAME_LENGTH - 1] = '\0';

  // Check if this is the admin user from .env
  char *adminUsername = envConfig.adminUsername;
  if (adminUsername && strcmp(username, adminUsername) == 0) {
    user->isAdmin = 1;
    // Admin doesn't get folders
    strcpy(user->userDir, "");
    strcpy(user->hashFile, "");
    printf("🔑 Login sebagai Admin: %s (Tanpa folder pribadi)\n", username);
  } else {
    user->isAdmin = 0;
    snprintf(user->userDir, sizeof(user->userDir), "%s/%s", USERS_DIR,
             username);
    snprintf(user->hashFile, sizeof(user->hashFile), "%s/%s/%s", USERS_DIR,
             username, HASH_FILENAME);
    printf("👤 Login sebagai User: %s\n", username);
  }

  return user;
}

// Destroy user session
void destroyUserSession() {
  if (currentUser != NULL) {
    free(currentUser);
    currentUser = NULL;
  }
}

// Register a new regular user (with personal folders)
int registerUser(const char *username, const char *password) {
  // Validate input
  if (username == NULL || password == NULL || strlen(username) == 0 ||
      strlen(password) == 0) {
    printf("Username dan password tidak boleh kosong.\n");
    return 0;
  }

  if (strlen(username) >= MAX_USERNAME_LENGTH) {
    printf("Username terlalu panjang (maksimal %d karakter).\n",
           MAX_USERNAME_LENGTH - 1);
    return 0;
  }

  if (strlen(password) >= MAX_PASSWORD_LENGTH) {
    printf("Password terlalu panjang (maksimal %d karakter).\n",
           MAX_PASSWORD_LENGTH - 1);
    return 0;
  }

  // Check if user already exists
  if (userExists(username)) {
    printf("Pengguna '%s' sudah ada.\n", username);
    return 0;
  }

  // Create user directory
  if (!createUserDirectory(username)) {
    printf("Gagal membuat direktori pengguna.\n");
    return 0;
  }

  // Create user reports directory for regular users only
  if (!createUserReportsDirectory(username)) {
    printf("Gagal membuat direktori laporan pengguna.\n");
    return 0;
  }

  // Hash password and save
  char *hash = hashPassword(password);
  if (hash == NULL) {
    printf("Gagal mengenkripsi password.\n");
    return 0;
  }

  if (!savePasswordHash(username, hash)) {
    printf("Gagal menyimpan password.\n");
    return 0;
  }

  printf("Pengguna '%s' berhasil didaftarkan!\n", username);
  return 1;
}

// Login user
int loginUser(const char *username, const char *password) {
  // Validate input
  if (username == NULL || password == NULL || strlen(username) == 0 ||
      strlen(password) == 0) {
    printf("Username dan password tidak boleh kosong.\n");
    return 0;
  }

  // Check if this is admin login
  char *adminUsername = envConfig.adminUsername;
  char *adminPassword = envConfig.adminPassword;

  if (adminUsername && adminPassword && strcmp(username, adminUsername) == 0 &&
      strcmp(password, adminPassword) == 0) {
    // Admin login successful
    currentUser = createUserSession(username);
    if (currentUser == NULL) {
      printf("Gagal membuat sesi admin.\n");
      return 0;
    }
    printf("Login admin berhasil! Selamat datang, %s.\n", username);
    return 1;
  }

  // Regular user login
  if (!userExists(username)) {
    printf("Pengguna '%s' tidak ditemukan.\n", username);
    return 0;
  }

  // Load stored hash
  char *storedHash = loadPasswordHash(username);
  if (storedHash == NULL) {
    printf("Gagal memuat kredensial pengguna.\n");
    return 0;
  }

  // Verify password
  if (!verifyPassword(password, storedHash)) {
    printf("Password salah.\n");
    return 0;
  }

  // Create user session
  currentUser = createUserSession(username);
  if (currentUser == NULL) {
    printf("Gagal membuat sesi pengguna.\n");
    return 0;
  }

  printf("Login berhasil! Selamat datang, %s.\n", username);
  return 1;
}

// Check if user is admin
int isUserAdmin(const char *username) {
  char *adminUsername = envConfig.adminUsername;
  return (adminUsername && strcmp(username, adminUsername) == 0) ? 1 : 0;
}

// Admin is only in .env, no creation needed
int createAdminUser(const char *username, const char *password) {
  printf("❌ Admin hanya di file .env!\n");
  return 0;
}

// Delete user and all their data
int deleteUser(const char *username) {
  // Can't delete admin
  char *adminUsername = envConfig.adminUsername;
  if (adminUsername && strcmp(username, adminUsername) == 0) {
    printf("❌ Tidak dapat menghapus admin!\n");
    return 0;
  }

  if (!userExists(username)) {
    printf("Pengguna '%s' tidak ditemukan.\n", username);
    return 0;
  }

  if (currentUser != NULL && strcmp(currentUser->username, username) == 0) {
    printf("Tidak dapat menghapus pengguna yang sedang login.\n");
    return 0;
  }

  char userDir[256];
  snprintf(userDir, sizeof(userDir), "%s/%s", USERS_DIR, username);

  char hashPath[256], configPath[256], reportsDir[256];
  snprintf(hashPath, sizeof(hashPath), "%s/password.hash", userDir);
  snprintf(configPath, sizeof(configPath), "%s/config.txt", userDir);
  snprintf(reportsDir, sizeof(reportsDir), "%s/reports", userDir);

  remove(hashPath);
  remove(configPath);
  rmdir(reportsDir);

  if (rmdir(userDir) == 0) {
    printf("Pengguna '%s' berhasil dihapus.\n", username);
    return 1;
  } else {
    printf("File pengguna dihapus tetapi direktori mungkin tidak kosong.\n");
    return 1;
  }
}

// Change user password (admin function)
int changeUserPassword(const char *username, const char *newPassword) {
  char *adminUsername = envConfig.adminUsername;
  if (adminUsername && strcmp(username, adminUsername) == 0) {
    printf("❌ Password admin hanya di file .env!\n");
    return 0;
  }

  if (!userExists(username)) {
    printf("Pengguna '%s' tidak ditemukan.\n", username);
    return 0;
  }

  char *hash = hashPassword(newPassword);
  if (hash == NULL) {
    printf("Gagal mengenkripsi password.\n");
    return 0;
  }

  if (savePasswordHash(username, hash)) {
    printf("Password berhasil diubah untuk '%s'.\n", username);
    return 1;
  }

  printf("Gagal menyimpan password.\n");
  return 0;
}

struct UserList *getAllUsers() {
  struct UserList *userList =
      (struct UserList *)malloc(sizeof(struct UserList));
  if (userList == NULL) {
    return NULL;
  }

  userList->usernames = NULL;
  userList->count = 0;

  DIR *dir = opendir(USERS_DIR);
  if (dir == NULL) {
    return userList;
  }

  struct dirent *entry;
  while ((entry = readdir(dir)) != NULL) {
    if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
      continue;
    }

    // Check if it's a directory
    char fullPath[256];
    snprintf(fullPath, sizeof(fullPath), "%s/%s", USERS_DIR, entry->d_name);

    struct stat st;
    if (stat(fullPath, &st) == 0 && S_ISDIR(st.st_mode)) {
      // Reallocate array
      char **temp = (char **)realloc(userList->usernames,
                                     (userList->count + 1) * sizeof(char *));
      if (temp == NULL) {
        break;
      }

      userList->usernames = temp;
      userList->usernames[userList->count] =
          (char *)malloc(strlen(entry->d_name) + 1);
      if (userList->usernames[userList->count] != NULL) {
        strcpy(userList->usernames[userList->count], entry->d_name);
        userList->count++;
      }
    }
  }

  closedir(dir);
  return userList;
}

void freeUserList(struct UserList *userList) {
  if (userList == NULL) {
    return;
  }

  if (userList->usernames != NULL) {
    for (int i = 0; i < userList->count; i++) {
      free(userList->usernames[i]);
    }
    free(userList->usernames);
  }

  free(userList);
}

// Create admin user session
struct User *createAdminUserSession(const char *username) {
  struct User *user = createUserSession(username);
  if (user != NULL && user->isAdmin) {
    printf("👑 Session admin aktif untuk: %s\n", username);
  } else if (user != NULL) {
    printf("❌ Pengguna '%s' bukan admin!\n", username);
    free(user);
    return NULL;
  }
  return user;
}

// Logout user
void logoutUser() {
  if (currentUser != NULL) {
    printf("Sampai jumpa, %s!\n", currentUser->username);
    destroyUserSession();
  }
}

// Clear authentication screen
void clearAuthScreen() { system("clear || cls"); }

// Show authentication menu
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

// Show user management menu
void showUserManagementMenu() {
  printf("╔══════════════════════════════════════════════════════════╗\n");
  printf("║                  👥 KELOLA PENGGUNA                     ║\n");
  printf("╠══════════════════════════════════════════════════════════╣\n");
  printf("║  1. 📋 Daftar Semua Pengguna                            ║\n");
  printf("║  2. ➕ Buat Pengguna Baru                               ║\n");
  printf("║  3. 🔑 Reset Password Pengguna                          ║\n");
  printf("║  4. 🗑️  Hapus Pengguna                                 ║\n");
  printf("║  5. ⬅️  Kembali                                         ║\n");
  printf("╚══════════════════════════════════════════════════════════╝\n");
  printf("💡 Admin hanya dapat dikelola melalui file .env\n");
  printf("Masukkan pilihan Anda (1-5): ");
}

// Show login menu
void showLoginMenu() {
  printf("╔══════════════════════════════════════════════════════════╗\n");
  printf("║                       🔐 MASUK                          ║\n");
  printf("╚══════════════════════════════════════════════════════════╝\n");
}

// Show register menu
void showRegisterMenu() {
  printf("╔══════════════════════════════════════════════════════════╗\n");
  printf("║                      📝 DAFTAR                          ║\n");
  printf("╚══════════════════════════════════════════════════════════╝\n");
}

// Handle login
int handleLogin() {
  char username[MAX_USERNAME_LENGTH];
  char password[MAX_PASSWORD_LENGTH];

  clearAuthScreen();
  showLoginMenu();

  if (!readSecureUsername("Username: ", username, sizeof(username))) {
    printf("❌ Error membaca username.\n");
    return 0;
  }

  if (!readSecurePassword("Password: ", password, sizeof(password))) {
    printf("❌ Error membaca password.\n");
    clearPassword(password, sizeof(password));
    return 0;
  }

  int result = loginUser(username, password);

  // Clear password from memory after use
  clearPassword(password, sizeof(password));

  return result;
}

// Handle register
int handleRegister() {
  char username[MAX_USERNAME_LENGTH];
  char password[MAX_PASSWORD_LENGTH];

  clearAuthScreen();
  showRegisterMenu();

  if (!readSecureUsername("Pilih username: ", username, sizeof(username))) {
    printf("❌ Error membaca username.\n");
    return 0;
  }

  if (!readSecurePasswordWithConfirmation(
          "Pilih password: ", "Konfirmasi password: ", password,
          sizeof(password))) {
    printf("❌ Error membaca atau password tidak cocok.\n");
    clearPassword(password, sizeof(password));
    return 0;
  }

  // Show password strength
  int strength = validatePasswordStrength(password);
  printf("💪 Kekuatan Password: %s\n",
         getPasswordStrengthDescription(strength));

  int result = registerUser(username, password);

  // Clear password from memory after use
  clearPassword(password, sizeof(password));

  return result;
}

// Handle admin login
int handleAdminLogin() {
  char username[MAX_USERNAME_LENGTH];
  char password[MAX_PASSWORD_LENGTH];

  clearAuthScreen();
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
  if (loginUser(username, password)) {
    if (currentUser && currentUser->isAdmin) {
      printf("✅ Akses admin diberikan!\n");
      result = 2; // Special return code for admin login
    } else {
      printf("❌ Akses ditolak: Pengguna '%s' bukan administrator.\n",
             username);
      logoutUser();
      result = 0;
    }
  }

  // Clear password from memory after use
  clearPassword(password, sizeof(password));

  return result;
}

// Handle admin user management
int handleAdminUserManagement() {
  while (1) {
    clearAuthScreen();
    showUserManagementMenu();

    int choice;
    if (scanf("%d", &choice) != 1) {
      int c;
      while ((c = getchar()) != '\n' && c != EOF)
        ;
      continue;
    }

    int c;
    while ((c = getchar()) != '\n' && c != EOF)
      ;

    switch (choice) {
    case 1: {
      // List all users
      struct UserList *userList = getAllUsers();
      printf(
          "\n╔══════════════════════════════════════════════════════════╗\n");
      printf("║                  👥 SEMUA PENGGUNA                      ║\n");
      printf("╠══════════════════════════════════════════════════════════╣\n");

      if (userList->count == 0) {
        printf("║  Tidak ada pengguna ditemukan.                          ║\n");
      } else {
        char *adminUsername = envConfig.adminUsername;
        for (int i = 0; i < userList->count; i++) {
          int isAdmin = (adminUsername &&
                         strcmp(userList->usernames[i], adminUsername) == 0);
          printf("║  %-30s %s                     ║\n", userList->usernames[i],
                 isAdmin ? "[ADMIN]" : "[USER] ");
        }
      }

      printf("╚══════════════════════════════════════════════════════════╝\n");
      freeUserList(userList);
      printf("\nTekan Enter untuk melanjutkan...");
      getchar();
      break;
    }
    case 2:
      return handleAdminUserCreation();
    case 3:
      return handlePasswordReset();
    case 4:
      return handleUserDeletion();
    case 5:
      return 1;
    default:
      printf("Pilihan tidak valid. Silakan coba lagi.\n");
      printf("Tekan Enter untuk melanjutkan...");
      getchar();
    }
  }
}

// Handle user deletion
int handleUserDeletion() {
  char username[MAX_USERNAME_LENGTH];
  char confirmation[10];

  printf("Masukkan username yang akan dihapus: ");
  if (fgets(username, sizeof(username), stdin) == NULL) {
    printf("Error membaca username.\n");
    return 0;
  }

  size_t len = strlen(username);
  if (len > 0 && username[len - 1] == '\n') {
    username[len - 1] = '\0';
  }

  printf("Apakah Anda yakin ingin menghapus pengguna '%s'? (yes/no): ",
         username);
  if (fgets(confirmation, sizeof(confirmation), stdin) == NULL) {
    printf("Error membaca konfirmasi.\n");
    return 0;
  }

  len = strlen(confirmation);
  if (len > 0 && confirmation[len - 1] == '\n') {
    confirmation[len - 1] = '\0';
  }

  if (strcmp(confirmation, "yes") == 0) {
    deleteUser(username);
  } else {
    printf("Penghapusan pengguna dibatalkan.\n");
  }

  printf("Tekan Enter untuk melanjutkan...");
  getchar();
  return 1;
}

// Handle password reset
int handlePasswordReset() {
  char username[MAX_USERNAME_LENGTH];
  char newPassword[MAX_PASSWORD_LENGTH];

  if (!readSecureUsername("Masukkan username untuk reset password: ", username,
                          sizeof(username))) {
    printf("❌ Error membaca username.\n");
    return 0;
  }

  if (!readSecurePasswordWithConfirmation(
          "Masukkan password baru: ", "Konfirmasi password baru: ", newPassword,
          sizeof(newPassword))) {
    printf("❌ Error membaca password atau password tidak cocok.\n");
    clearPassword(newPassword, sizeof(newPassword));
    return 0;
  }

  // Show password strength
  int strength = validatePasswordStrength(newPassword);
  printf("💪 Kekuatan Password: %s\n",
         getPasswordStrengthDescription(strength));

  int result = changeUserPassword(username, newPassword);

  // Clear password from memory after use
  clearPassword(newPassword, sizeof(newPassword));

  return result;
}

// Handle admin user creation
int handleAdminUserCreation() {
  char username[MAX_USERNAME_LENGTH];
  char password[MAX_PASSWORD_LENGTH];

  if (!readSecureUsername("Masukkan username admin baru: ", username,
                          sizeof(username))) {
    printf("❌ Error membaca username.\n");
    return 0;
  }

  if (!readSecurePasswordWithConfirmation(
          "Masukkan password admin: ", "Konfirmasi password admin: ", password,
          sizeof(password))) {
    printf("❌ Error membaca password atau password tidak cocok.\n");
    clearPassword(password, sizeof(password));
    return 0;
  }

  // Show password strength
  int strength = validatePasswordStrength(password);
  printf("💪 Kekuatan Password: %s\n",
         getPasswordStrengthDescription(strength));

  int result = createAdminUser(username, password);

  // Clear password from memory after use
  clearPassword(password, sizeof(password));

  return result;
}

// Handle authentication menu choice
int handleAuthMenuChoice() {
  int choice;

  if (scanf("%d", &choice) != 1) {
    // Clear input buffer
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
      ;
    return -1;
  }

  // Clear input buffer
  int c;
  while ((c = getchar()) != '\n' && c != EOF)
    ;

  switch (choice) {
  case 1:
    return handleLogin();
  case 2:
    return handleRegister();
  case 3:
    return handleAdminLogin();
  case 4:
    printf("Sampai jumpa!\n");
    exit(0);
  default:
    printf("Pilihan tidak valid. Silakan coba lagi.\n");
    return 0;
  }
}
