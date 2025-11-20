#include "auth/user_reports.c"
#include "db/month_report_list.c"
#include "ui/include.c"
#include <dirent.h>

int main() {
  if (!initializeAuthSystem()) {
    printf("Failed to initialize authentication system.\n");
    return 1;
  }

  while (currentUser == NULL) {
    clearScreen();
    showAuthMenu();

    int authResult = handleAuthMenuChoice();
    if (authResult == 0) {
      printf("\nPress Enter to continue...");
      getchar();
    }
  }
  initializeDefaultConfig();
  if (currentUser != NULL && !currentUser->isAdmin) {
    char userConfigPath[512];
    snprintf(userConfigPath, sizeof(userConfigPath), "%s/%s/config.txt",
             USERS_DIR, currentUser->username);
    loadConfigFromFile(userConfigPath);
  } else {
    loadConfigFromFile("./config.txt");
  }

  struct MonthReportList *monthReportList;
  if (currentUser != NULL && !currentUser->isAdmin) {
    // For regular users, load their own reports
    monthReportList = listUserMonthReports();
  } else {
    // For admin, load all users' reports
    monthReportList = listAllUsersReports();
  }

  // Ensure we have a valid list
  if (monthReportList == NULL) {
    monthReportList = createMonthReportList();
  }

  openMainMenu(monthReportList);

  if (currentUser != NULL && !currentUser->isAdmin) {
    char userConfigPath[512];
    snprintf(userConfigPath, sizeof(userConfigPath), "%s/%s/config.txt",
             USERS_DIR, currentUser->username);
    saveConfigToFile(userConfigPath);
  } else {
    saveConfigToFile("./config.txt");
  }

  logoutUser();

  return 0;
}
