#include "../auth/auth.h"
#include "../types/include.h"
#include "../ui/master.h"
#include "../utils/env_loader.c"
#include "./month_report_list.c"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#ifndef DB_USER_MANAGEMENT_C
#define DB_USER_MANAGEMENT_C

char *getUserConfigPath() {
  if (currentUser == NULL || currentUser->isAdmin) {
    return NULL;
  }

  static char configPath[256];
  snprintf(configPath, sizeof(configPath), "%s/%s/config.txt", USERS_DIR,
           currentUser->username);
  return configPath;
}

void createDefaultUserConfig() {
  if (currentUser == NULL || currentUser->isAdmin) {
    return;
  }

  char *configPath = getUserConfigPath();
  if (configPath == NULL) {
    return;
  }

  FILE *file = fopen(configPath, "r");
  if (file != NULL) {
    fclose(file);
    return;
  }

  file = fopen(configPath, "w");
  if (file != NULL) {
    fprintf(file, "# User Configuration File\n");
    fprintf(file, "# Generated automatically for user: %s\n",
            currentUser->username);
    fprintf(file, "MINIMUM_AMOUNT=1000\n");
    fprintf(file, "MINIMUM_BUDGET=10000\n");
    fprintf(file, "MINIMUM_TRANSACTION=500\n");
    fclose(file);
  }
}

int initializeUserWorkspace(const char *username) {
  if (!createUserDirectory(username)) {
    return 0;
  }

  if (!createUserReportsDirectory(username)) {
    return 0;
  }

  struct User *tempUser = createUserSession(username);
  if (tempUser == NULL) {
    return 0;
  }

  struct User *oldUser = currentUser;
  currentUser = tempUser;

  createDefaultUserConfig();

  struct CategoryList *categories = initializeDefaultCategories();
  if (categories != NULL) {
    saveUserCategories(categories);
    freeCategoryList(categories);
  }

  currentUser = oldUser;
  free(tempUser);

  return 1;
}

void cleanupUserSession() {
  if (currentUser != NULL) {
    if (!currentUser->isAdmin) {
      char *configPath = getUserConfigPath();
      if (configPath != NULL) {
        saveConfigToFile(configPath);
      }
    }

    destroyUserSession();
  }
}

void resetUserBudgets(struct MonthReportList *monthReportList) {
  if (monthReportList == NULL || currentUser == NULL || currentUser->isAdmin) {
    return;
  }

  struct CategoryList *categories = getUserCategoriesCache();
  if (categories == NULL) {
    return;
  }

  for (int i = 0; i < monthReportList->count; i++) {
    struct MonthReport *report = monthReportList->reports[i];

    for (int j = 0; j < categories->count; j++) {
      if (categories->items[j] != NULL) {
        setBudgetForCategory(report, categories->items[j]->internalName,
                             1000000);
      }
    }

    saveUserMonthReport(report);
  }
}

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

int forcePasswordReset(const char *targetUsername) {
  if (!validateAdminPrivileges("force password reset")) {
    return 0;
  }

  if (!userExists(targetUsername)) {
    printf("❌ Pengguna '%s' tidak ditemukan!\n", targetUsername);
    return 0;
  }

  char newPassword[256];
  printf("🔐 Reset password untuk pengguna: %s\n", targetUsername);

  printf("Password baru: ");
  if (scanf("%255s", newPassword) == 1) {
    if (changeUserPassword(targetUsername, newPassword)) {
      printf("✅ Password berhasil direset untuk '%s'!\n", targetUsername);
      return 1;
    } else {
      printf("❌ Gagal mereset password!\n");
    }
  }

  return 0;
}

#endif
