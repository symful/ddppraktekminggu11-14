#include "ui/include.c"
#include <dirent.h>

int main() {
  initializeDefaultConfig();
  loadConfigFromFile("./config.txt");

  struct MonthReportList *monthReportList =
      listAllMonthReports(opendir("./reports"));

  openMainMenu(monthReportList);

  saveConfigToFile("./config.txt");
  return 0;
}
