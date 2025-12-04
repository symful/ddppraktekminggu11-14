#include "auth/user_reports.c"
#include "db/month_report_list.c"
#include "ui/include.c"
#include <dirent.h>

void handleUserAuthentication() {
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

void loadUserConfiguration() {
  initializeDefaultConfig();

  if (currentUser != NULL && !currentUser->isAdmin) {
    char userConfigPath[512];
    snprintf(userConfigPath, sizeof(userConfigPath), "%s/%s/config.txt",
             USERS_DIR, currentUser->username);
    loadConfigFromFile(userConfigPath);
  } else {
    loadConfigFromFile("./config.txt");
  }
}

struct MonthReportList *loadUserReports() {
  struct MonthReportList *list;

  if (currentUser != NULL && !currentUser->isAdmin) {
    list = listUserMonthReports();
  } else {
    list = listAllUsersReports();
  }

  if (list == NULL) {
    list = createMonthReportList();
  }

  return list;
}

void saveUserConfiguration() {
  if (currentUser != NULL && !currentUser->isAdmin) {
    char userConfigPath[512];
    snprintf(userConfigPath, sizeof(userConfigPath), "%s/%s/config.txt",
             USERS_DIR, currentUser->username);
    saveConfigToFile(userConfigPath);
  } else {
    saveConfigToFile("./config.txt");
  }
}

void runApplication() {

  if (!initializeAuthSystem()) {
    printf("Failed to initialize authentication system.\n");
    return;
  }

  handleUserAuthentication();
  loadUserConfiguration();

  struct MonthReportList *monthReportList = loadUserReports();

  openMainMenu(monthReportList);

  saveUserConfiguration();
  logoutUser();
}

/* =======================================================
    PROGRAM MAIN
   =======================================================*/
int main() {
  runApplication();
  return 0;
}
