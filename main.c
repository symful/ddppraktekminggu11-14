#include "ui/include.c"
#include <dirent.h>

int main() {
  struct MonthReportList *monthReportList =
      listAllMonthReports(opendir("./reports"));

  openMainMenu(monthReportList);
  return 0;
}
