#include "./month_report.h"

#ifndef MONTH_REPORT_LIST_STRUCT_DEFINED
#define MONTH_REPORT_LIST_STRUCT_DEFINED

struct MonthReportList {
    int count;                         // Changed from amount for consistency
    struct MonthReport** reports;      // Array of pointers to MonthReport structures
};

#endif