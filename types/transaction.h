#include <time.h>
#include "./transaction_type.h"

struct Transaction {
    int id;
    time_t date;
    char name[20];
    long long maximumCost;
    char description[200];
    enum TransactionType type;
    long long realCost;
    long long restCost;
    long long amount;
};
