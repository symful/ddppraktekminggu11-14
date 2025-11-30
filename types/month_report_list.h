#include "./month_report.h"

#ifndef MONTH_REPORT_LIST_STRUCT_DEFINED
#define MONTH_REPORT_LIST_STRUCT_DEFINED

struct MonthReportList {
    int count;
    struct MonthReport** reports;
};

#endif
