#include <ctype.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#ifndef USER_MANAGER_C
#define USER_MANAGER_C

#define MAX_USERNAME_LENGTH 50
#define MAX_PASSWORD_LENGTH 100
#define USERS_DIR "users"

// User structure
struct User {
  char username[MAX_USERNAME_LENGTH];
  char userDir[256];
  char reportsDir[256];
};

// User list structure for admin operations
struct UserList {
  char **usernames;
  int count;
};

// Create users directory if it doesn't exist
int createUsersDirectory() {
  struct stat st = {0};
  if (stat(USERS_DIR, &st) == -1) {
    if (mkdir(USERS_DIR, 0700) != 0) {
      perror("Error creating users directory");
      return 0;
    }
  }
  return 1;
}

// Check if user exists
int userExists(const char *username) {
  if (!username)
    return 0;

  char userDir[256];
  snprintf(userDir, sizeof(userDir), "%s/%s", USERS_DIR, username);

  struct stat st = {0};
  return (stat(userDir, &st) == 0 && S_ISDIR(st.st_mode));
}

// Create user directory
int createUserDirectory(const char *username) {
  if (!username)
    return 0;

  char userDir[256];
  snprintf(userDir, sizeof(userDir), "%s/%s", USERS_DIR, username);

  if (mkdir(userDir, 0700) != 0) {
    perror("Error creating user directory");
    return 0;
  }
  return 1;
}

// Create user reports directory
int createUserReportsDirectory(const char *username) {
  if (!username)
    return 0;

  char reportsDir[256];
  snprintf(reportsDir, sizeof(reportsDir), "%s/%s/reports", USERS_DIR,
           username);

  if (mkdir(reportsDir, 0700) != 0) {
    perror("Error creating user reports directory");
    return 0;
  }
  return 1;
}

// Validate username
int isValidUsername(const char *username) {
  if (!username || strlen(username) == 0) {
    printf("Username tidak boleh kosong.\n");
    return 0;
  }

  if (strlen(username) >= MAX_USERNAME_LENGTH) {
    printf("Username terlalu panjang (maksimal %d karakter).\n",
           MAX_USERNAME_LENGTH - 1);
    return 0;
  }

  // Check for invalid characters
  for (int i = 0; username[i]; i++) {
    if (!isalnum(username[i]) && username[i] != '_' && username[i] != '-') {
      printf("Username hanya boleh mengandung huruf, angka, underscore, dan "
             "dash.\n");
      return 0;
    }
  }

  return 1;
}

// Validate password
int isValidPassword(const char *password) {
  if (!password || strlen(password) == 0) {
    printf("Password tidak boleh kosong.\n");
    return 0;
  }

  if (strlen(password) >= MAX_PASSWORD_LENGTH) {
    printf("Password terlalu panjang (maksimal %d karakter).\n",
           MAX_PASSWORD_LENGTH - 1);
    return 0;
  }

  if (strlen(password) < 6) {
    printf("Password minimal 6 karakter.\n");
    return 0;
  }

  return 1;
}

// Save password to file (plain text for simplicity)
int saveUserPassword(const char *username, const char *password) {
  if (!username || !password)
    return 0;

  char passwordFile[256];
  snprintf(passwordFile, sizeof(passwordFile), "%s/%s/password.txt", USERS_DIR,
           username);

  FILE *file = fopen(passwordFile, "w");
  if (!file) {
    perror("Error saving password");
    return 0;
  }

  fprintf(file, "%s", password);
  fclose(file);
  return 1;
}

// Load password from file
int loadUserPassword(const char *username, char *password, size_t maxLen) {
  if (!username || !password)
    return 0;

  char passwordFile[256];
  snprintf(passwordFile, sizeof(passwordFile), "%s/%s/password.txt", USERS_DIR,
           username);

  FILE *file = fopen(passwordFile, "r");
  if (!file) {
    return 0;
  }

  if (fgets(password, maxLen, file) == NULL) {
    fclose(file);
    return 0;
  }

  // Remove newline if present
  size_t len = strlen(password);
  if (len > 0 && password[len - 1] == '\n') {
    password[len - 1] = '\0';
  }

  fclose(file);
  return 1;
}

// Create new user
int createUser(const char *username, const char *password) {
  if (!isValidUsername(username)) {
    return 0;
  }

  if (!isValidPassword(password)) {
    return 0;
  }

  if (userExists(username)) {
    printf("Pengguna '%s' sudah ada.\n", username);
    return 0;
  }

  if (!createUserDirectory(username)) {
    printf("Gagal membuat direktori pengguna.\n");
    return 0;
  }

  if (!createUserReportsDirectory(username)) {
    printf("Gagal membuat direktori laporan pengguna.\n");
    return 0;
  }

  if (!saveUserPassword(username, password)) {
    printf("Gagal menyimpan password pengguna.\n");
    return 0;
  }

  printf("Pengguna '%s' berhasil dibuat!\n", username);
  return 1;
}

// Authenticate user
int authenticateUser(const char *username, const char *password) {
  if (!username || !password) {
    printf("Username dan password tidak boleh kosong.\n");
    return 0;
  }

  if (!userExists(username)) {
    printf("Pengguna '%s' tidak ditemukan.\n", username);
    return 0;
  }

  char storedPassword[MAX_PASSWORD_LENGTH];
  if (!loadUserPassword(username, storedPassword, sizeof(storedPassword))) {
    printf("Gagal memuat password pengguna.\n");
    return 0;
  }

  if (strcmp(password, storedPassword) != 0) {
    printf("Password salah.\n");
    return 0;
  }

  return 1;
}

// Delete user and all data
int deleteUser(const char *username) {
  if (!username)
    return 0;

  if (!userExists(username)) {
    printf("Pengguna '%s' tidak ditemukan.\n", username);
    return 0;
  }

  char userDir[256];
  snprintf(userDir, sizeof(userDir), "%s/%s", USERS_DIR, username);

  // Remove password file
  char passwordFile[256];
  snprintf(passwordFile, sizeof(passwordFile), "%s/password.txt", userDir);
  remove(passwordFile);

  // Remove reports directory (simplified - remove known files)
  char reportsDir[256];
  snprintf(reportsDir, sizeof(reportsDir), "%s/reports", userDir);
  rmdir(reportsDir);

  // Remove user directory
  if (rmdir(userDir) == 0) {
    printf("Pengguna '%s' berhasil dihapus.\n", username);
    return 1;
  } else {
    printf("Pengguna dihapus tetapi direktori mungkin tidak kosong.\n");
    return 1;
  }
}

// Change user password
int changeUserPassword(const char *username, const char *newPassword) {
  if (!username || !newPassword)
    return 0;

  if (!userExists(username)) {
    printf("Pengguna '%s' tidak ditemukan.\n", username);
    return 0;
  }

  if (!isValidPassword(newPassword)) {
    return 0;
  }

  if (saveUserPassword(username, newPassword)) {
    printf("Password berhasil diubah untuk pengguna '%s'.\n", username);
    return 1;
  }

  printf("Gagal mengubah password.\n");
  return 0;
}

// Get list of all users
struct UserList *getAllUsers() {
  struct UserList *userList =
      (struct UserList *)malloc(sizeof(struct UserList));
  if (!userList)
    return NULL;

  userList->usernames = NULL;
  userList->count = 0;

  DIR *dir = opendir(USERS_DIR);
  if (!dir) {
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
      if (!temp)
        break;

      userList->usernames = temp;
      userList->usernames[userList->count] =
          (char *)malloc(strlen(entry->d_name) + 1);
      if (userList->usernames[userList->count]) {
        strcpy(userList->usernames[userList->count], entry->d_name);
        userList->count++;
      }
    }
  }

  closedir(dir);
  return userList;
}

// Free user list
void freeUserList(struct UserList *userList) {
  if (!userList)
    return;

  if (userList->usernames) {
    for (int i = 0; i < userList->count; i++) {
      free(userList->usernames[i]);
    }
    free(userList->usernames);
  }

  free(userList);
}

// Get user's reports directory path
char *getUserReportsPath(const char *username) {
  if (!username)
    return NULL;

  static char reportsPath[256];
  snprintf(reportsPath, sizeof(reportsPath), "%s/%s/reports", USERS_DIR,
           username);
  return reportsPath;
}

// Create user session structure
struct User *createUserSession(const char *username) {
  if (!username || !userExists(username))
    return NULL;

  struct User *user = (struct User *)malloc(sizeof(struct User));
  if (!user)
    return NULL;

  strncpy(user->username, username, sizeof(user->username) - 1);
  user->username[sizeof(user->username) - 1] = '\0';

  snprintf(user->userDir, sizeof(user->userDir), "%s/%s", USERS_DIR, username);
  snprintf(user->reportsDir, sizeof(user->reportsDir), "%s/%s/reports",
           USERS_DIR, username);

  return user;
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
  printf("║  5. 🔙 Kembali ke Menu Admin                            ║\n");
  printf("╚══════════════════════════════════════════════════════════╝\n");
  printf("Masukkan pilihan Anda (1-5): ");
}

#endif
