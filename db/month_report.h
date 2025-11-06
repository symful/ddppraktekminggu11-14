#include "../types/month_report.h"
#include <stdio.h>

void saveMonthReport(MonthReport* report, FILE* file);

struct MonthReport loadMonthReport(FILE *file);
