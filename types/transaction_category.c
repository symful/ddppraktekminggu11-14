#include "./transaction_category.h"

#ifndef TRANSACTION_CATEGORY_C
#define TRANSACTION_CATEGORY_C

const char *transactionCategoryToString(enum TransactionCategory category) {
  switch (category) {
  case TC_FOOD:
    return "Makanan";
  case TC_TRANSPORTATION:
    return "Transportasi";
  case TC_ENTERTAINMENT:
    return "Hiburan";
  case TC_INCOME:
    return "Pendapatan";
  case TC_WORK:
    return "Pekerjaan";
  case TC_SHOPPING:
    return "Belanja";
  case TC_OTHER:
    return "Lainnya";
  default:
    return "Tidak diketahui";
  }
}

#endif
