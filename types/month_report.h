#include "./transaction_group.h"
#include <time.h>

#ifndef MONTH_REPORT_STRUCT_DEFINED
#define MONTH_REPORT_STRUCT_DEFINED

struct MonthReport {
    time_t date;
    int groupCount;                    // Changed from groupsAmount for consistency
    struct TransactionGroup** groups; // Changed to array of pointers for better memory management
    long long totalIncome;
    long long totalExpenses;           // Changed from totalExpense for consistency
    long long balance;
};

#endif