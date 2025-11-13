#include "./transaction.h"

#ifndef MONTH_REPORT_STRUCT_DEFINED
#define MONTH_REPORT_STRUCT_DEFINED

struct MonthReport {
    time_t date;
    int transactionsAmount;
    struct Transaction* transactions;
};

#endif
