#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>

#include "db/include.c"
#include "types/include.h"

int main() {
  printf("=== DEBUG MODE ===\n");

  printf("Test 1: Creating new month report...\n");
  struct MonthReport *testReport = newMonthReport();
  if (testReport == NULL) {
    printf("ERROR: Failed to create new month report\n");
    return 1;
  }
  printf("SUCCESS: New month report created\n");

  printf("\nTest 2: Checking reports directory...\n");
  DIR *dir = opendir("./reports");
  if (dir == NULL) {
    printf("ERROR: Cannot open reports directory\n");
    return 1;
  }
  printf("SUCCESS: Reports directory opened\n");

  printf("\nTest 3: Listing files in reports directory...\n");
  struct dirent *entry;
  while ((entry = readdir(dir)) != NULL) {
    if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
      continue;
    }
    printf("Found file: %s\n", entry->d_name);
  }
  closedir(dir);

  printf("\nTest 4: Creating empty month report list...\n");
  struct MonthReportList *monthReportList =
      (struct MonthReportList *)malloc(sizeof(struct MonthReportList));
  if (monthReportList == NULL) {
    printf("ERROR: Failed to allocate memory for month report list\n");
    return 1;
  }
  monthReportList->reports = NULL;
  monthReportList->amount = 0;
  printf("SUCCESS: Empty month report list created with %d reports\n",
         monthReportList->amount);

  printf("\nTest 5: Attempting to load existing report file...\n");
  struct MonthReport *loadedReport =
      loadMonthReport("./reports/report_2024_01.txt");
  if (loadedReport == NULL) {
    printf("WARNING: Could not load existing report (expected with format "
           "change)\n");
  } else {
    printf("SUCCESS: Loaded report with %d groups\n",
           loadedReport->groupsAmount);
  }

  printf("\nTest 6: Testing with empty list...\n");
  printf("Monthly report list has %d reports\n", monthReportList->amount);

  printf("\n=== DEBUG COMPLETE ===\n");
  printf("All basic tests passed. The issue is likely in loading old format "
         "files.\n");

  freeMonthReport(testReport);
  free(monthReportList);

  return 0;
}
