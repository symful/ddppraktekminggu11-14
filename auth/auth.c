
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

#ifndef CURRENT_USER
struct User *currentUser = NULL;

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

int initializeAuthSystem() {
  struct stat st = {0};

  if (stat(USERS_DIR, &st) == -1) {
    if (mkdir(USERS_DIR, 0700) != 0) {
      perror("Error creating users directory");
      return 0;
    }
  }

  printf("✅ Sistem autentikasi diinisialisasi.\n");

  return 1;
}

int userExists(const char *username) {
  char userDir[256];
  snprintf(userDir, sizeof(userDir), "%s/%s", USERS_DIR, username);

  struct stat st = {0};
  return (stat(userDir, &st) == 0);
}

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

int verifyPassword(const char *password, const char *storedHash) {
  char *hashedInput = hashPassword(password);
  if (hashedInput == NULL) {
    return 0;
  }

  return (strcmp(hashedInput, storedHash) == 0);
}

char *getUserReportsPath(const char *username) {
  static char reportsPath[256];
  snprintf(reportsPath, sizeof(reportsPath), "%s/%s/reports", USERS_DIR,
           username);
  return reportsPath;
}

struct User *createUserSession(const char *username) {
  struct User *user = (struct User *)malloc(sizeof(struct User));
  if (user == NULL) {
    return NULL;
  }

  strncpy(user->username, username, MAX_USERNAME_LENGTH - 1);
  user->username[MAX_USERNAME_LENGTH - 1] = '\0';

  char *adminUsername = envConfig.adminUsername;
  if (adminUsername && strcmp(username, adminUsername) == 0) {
    user->isAdmin = 1;

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

void destroyUserSession() {
  if (currentUser != NULL) {
    refreshCategoryCache();
    free(currentUser);
    currentUser = NULL;
  }
}

int registerUser(const char *username, const char *password) {

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

int loginUser(const char *username, const char *password) {

  if (username == NULL || password == NULL || strlen(username) == 0 ||
      strlen(password) == 0) {
    printf("Username dan password tidak boleh kosong.\n");
    return 0;
  }

  char *adminUsername = envConfig.adminUsername;
  char *adminPassword = envConfig.adminPassword;

  if (adminUsername && adminPassword && strcmp(username, adminUsername) == 0 &&
      strcmp(password, adminPassword) == 0) {

    currentUser = createUserSession(username);
    if (currentUser == NULL) {
      printf("Gagal membuat sesi admin.\n");
      return 0;
    }
    printf("Login admin berhasil! Selamat datang, %s.\n", username);
    return 1;
  }

  if (!userExists(username)) {
    printf("Pengguna '%s' tidak ditemukan.\n", username);
    return 0;
  }

  char *storedHash = loadPasswordHash(username);
  if (storedHash == NULL) {
    printf("Gagal memuat kredensial pengguna.\n");
    return 0;
  }

  if (!verifyPassword(password, storedHash)) {
    printf("Password salah.\n");
    return 0;
  }

  currentUser = createUserSession(username);
  if (currentUser == NULL) {
    printf("Gagal membuat sesi pengguna.\n");
    return 0;
  }

  printf("Login berhasil! Selamat datang, %s.\n", username);
  return 1;
}

int isUserAdmin(const char *username) {
  char *adminUsername = envConfig.adminUsername;
  return (adminUsername && strcmp(username, adminUsername) == 0) ? 1 : 0;
}

int createAdminUser(const char *username, const char *password) {
  printf("❌ Admin hanya di file .env!\n");
  return 0;
}

int deleteUser(const char *username) {

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

    char fullPath[256];
    snprintf(fullPath, sizeof(fullPath), "%s/%s", USERS_DIR, entry->d_name);

    struct stat st;
    if (stat(fullPath, &st) == 0 && S_ISDIR(st.st_mode)) {

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

void logoutUser() {
  if (currentUser != NULL) {
    printf("Sampai jumpa, %s!\n", currentUser->username);
    destroyUserSession();
  }
}

#define CURRENT_USER currentUser
#endif
