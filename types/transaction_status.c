#include "./transaction_status.h"

#ifndef TRANSACTION_STATUS_C
#define TRANSACTION_STATUS_C

const char *transactionStatusToString(enum TransactionStatus status) {
  switch (status) {
  case TRANSACTION_BUDGET_SAFE:
    return "Aman";
    break;
  case TRANSACTION_BUDGET_EXCEEDED:
    return "Melebihi";
    break;
  case TRANSACTION_BUDGET_WARNING:
    return "Peringatan";
    break;
  default:
    return "Tidak diketahui";
    break;
  }
}

#endif
