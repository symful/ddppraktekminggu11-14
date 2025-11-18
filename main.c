#include "auth/auth.c"
#include "auth/user_reports.c"
#include "ui/include.c"
#include <dirent.h>

int main() {
  // Initialize authentication system
  if (!initializeAuthSystem()) {
    printf("Failed to initialize authentication system.\n");
    return 1;
  }

  // Authentication loop
  while (currentUser == NULL) {
    clearAuthScreen();
    showAuthMenu();

    int authResult = handleAuthMenuChoice();
    if (authResult == 0) {
      printf("\nPress Enter to continue...");
      getchar();
    }
  }

  // User is now logged in - proceed with existing system
  initializeDefaultConfig();

  // Load user-specific config if exists, otherwise use default
  // Admin users don't have personal configs - they use system default
  if (currentUser != NULL && !currentUser->isAdmin) {
    char userConfigPath[512];
    snprintf(userConfigPath, sizeof(userConfigPath), "%s/%s/config.txt",
             USERS_DIR, currentUser->username);
    loadConfigFromFile(userConfigPath);
  } else {
    loadConfigFromFile("./config.txt");
  }

  // Load reports from user directory or default
  // Admin users don't have personal reports - they manage all users
  struct MonthReportList *monthReportList;
  if (currentUser != NULL && !currentUser->isAdmin) {
    char *userReportsPath = getUserReportsPath(currentUser->username);
    DIR *userReportsDir = opendir(userReportsPath);
    monthReportList = listAllMonthReports(userReportsDir);
  } else {
    // For admin users or no user, use default reports directory
    monthReportList = listAllMonthReports(opendir("./reports"));
  }

  // Use existing UI system
  openMainMenu(monthReportList);

  // Save config to user-specific location
  // Admin users save to system default, not personal config
  if (currentUser != NULL && !currentUser->isAdmin) {
    char userConfigPath[512];
    snprintf(userConfigPath, sizeof(userConfigPath), "%s/%s/config.txt",
             USERS_DIR, currentUser->username);
    saveConfigToFile(userConfigPath);
  } else {
    saveConfigToFile("./config.txt");
  }

  // Cleanup
  logoutUser();

  return 0;
}
