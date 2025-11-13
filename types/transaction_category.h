#ifndef TRANSACTION_CATEGORY_ENUM
#define TRANSACTION_CATEGORY_ENUM

enum TransactionCategory {
    TC_TRANSPORTATION,
    TC_INCOME,
    TC_WORK,
    TC_FOOD,
    TC_ENTERTAINMENT,
    TC_SHOPPING,
    TC_OTHER
};

const char *transactionCategoryToString(enum TransactionCategory category);
#endif
