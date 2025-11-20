#include <time.h>
#include "./transaction_type.h"
#include "./transaction_category.h"
#include "./transaction_status.h"

#ifndef TRANSACTION_STRUCT_DEFINED
#define TRANSACTION_STRUCT_DEFINED

struct Transaction {
    int id;
    time_t date;
    char name[50];                      // Increased size for better usability
    char description[256];              // Increased size for better usability
    enum TransactionType type;
    enum TransactionCategory category;
    enum TransactionStatus status;
    long long amount;                   // Main amount field - simplified from multiple cost fields
};

#endif