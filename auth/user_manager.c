#include "../types/include.c"
#include <ctype.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#ifndef USER_MANAGER_C
#define USER_MANAGER_C

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

  for (int i = 0; username[i]; i++) {
    if (!isalnum(username[i]) && username[i] != '_' && username[i] != '-') {
      printf("Username hanya boleh mengandung huruf, angka, underscore, dan "
             "dash.\n");
      return 0;
    }
  }

  return 1;
}

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

  size_t len = strlen(password);
  if (len > 0 && password[len - 1] == '\n') {
    password[len - 1] = '\0';
  }

  fclose(file);
  return 1;
}

char *getUserReportsPath(const char *username) {
  if (!username)
    return NULL;

  static char reportsPath[256];
  snprintf(reportsPath, sizeof(reportsPath), "%s/%s/reports", USERS_DIR,
           username);
  return reportsPath;
}

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
