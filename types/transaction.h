#include <time.h>
#include "./transaction_type.h"
#include "./transaction_category.h"
#ifndef TRANSACTION_STRUCT_DEFINED
#define TRANSACTION_STRUCT_DEFINED

struct Transaction {
    int id;
    time_t date;
    char name[20];
    long long maximumCost;
    char description[200];
    enum TransactionType type;
    enum TransactionCategory category;
    long long realCost;
    long long restCost;
    long long amount;
};

#endif
