#include "./transaction.h"

struct MonthReport {
    time_t date;
    int transactionsAmount;
    struct Transaction transactions[];
};
