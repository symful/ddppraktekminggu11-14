#include "../types/month_report.h"
#include <stdio.h>

void saveMonthReport(struct MonthReport *report, FILE *file) {
  fwrite(report, sizeof(struct MonthReport), 1, file);
}

struct MonthReport loadMonthReport(FILE *file) {
  struct MonthReport report;
  fread(&report, sizeof(struct MonthReport), 1, file);
  return report;
}
