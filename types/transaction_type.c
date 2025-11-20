#include "./transaction_type.h"

#ifndef TRANSACTION_TYPE_C
#define TRANSACTION_TYPE_C

const char *transactionTypeToString(enum TransactionType type) {
  switch (type) {
  case TRANSACTION_EXPENSE:
    return "Pengeluaran";
    break;
  case TRANSACTION_INCOME:
    return "Pendapatan";
    break;
  default:
    return "Tidak Diketahui";
    break;
  }
}

#endif
