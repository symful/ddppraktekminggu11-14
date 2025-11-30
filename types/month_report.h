#include "./transaction_group.h"
#include <time.h>

#ifndef MONTH_REPORT_STRUCT_DEFINED
#define MONTH_REPORT_STRUCT_DEFINED

struct MonthReport {
    time_t date;
    int groupCount;
    struct TransactionGroup** groups;
    long long totalIncome;
    long long totalExpenses;
    long long balance;
};

#endif
