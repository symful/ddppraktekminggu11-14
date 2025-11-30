#include <time.h>
#include "./transaction_type.h"
#include "./transaction_category.h"
#include "./transaction_status.h"

#ifndef TRANSACTION_STRUCT_DEFINED
#define TRANSACTION_STRUCT_DEFINED

struct Transaction {
    int id;
    time_t date;
    char name[50];
    char description[256];
    enum TransactionType type;
    char category[50];
    enum TransactionStatus status;
    long long amount;
};

#endif
