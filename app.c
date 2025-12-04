#include "auth/auth.h"
#include "auth/user_reports.c"
#include "config/config.h"
#include "db/month_report_list.h"
#include "ui/include.h"
#include <stdio.h>

void initializeUserConfig() {
  if (currentUser != NULL && !currentUser->isAdmin) {
    char userConfigPath[512];
    snprintf(userConfigPath, sizeof(userConfigPath), "%s/%s/config.txt",
             USERS_DIR, currentUser->username);
    loadConfigFromFile(userConfigPath);
  } else {
    loadConfigFromFile("./config.txt");
  }
}

void saveUserConfig() {
  if (currentUser != NULL && !currentUser->isAdmin) {
    char userConfigPath[512];
    snprintf(userConfigPath, sizeof(userConfigPath), "%s/%s/config.txt",
             USERS_DIR, currentUser->username);
    saveConfigToFile(userConfigPath);
  } else {
    saveConfigToFile("./config.txt");
  }
}

struct MonthReportList *loadMonthReports() {
  if (currentUser != NULL && !currentUser->isAdmin) {
    return listUserMonthReports();
  } else {
    return listAllUsersReports();
  }
}

void authenticateLoop() {
  while (currentUser == NULL) {
    clearScreen();
    showAuthMenu();

    int authResult = handleAuthMenuChoice();
    if (authResult == 0) {
      printf("\nPress Enter to continue...");
      getchar();
    }
  }
}

void runApplication() {
  if (!initializeAuthSystem()) {
    printf("Failed to initialize authentication system.\n");
    return;
  }

  authenticateLoop();

  initializeDefaultConfig();
  initializeUserConfig();

  struct MonthReportList *monthReportList = loadMonthReports();
  if (monthReportList == NULL) {
    monthReportList = createMonthReportList();
  }

  openMainMenu(monthReportList);

  saveUserConfig();
  logoutUser();
}
