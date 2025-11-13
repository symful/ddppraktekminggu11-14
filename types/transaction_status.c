#include "./transaction_status.h"

#ifndef TRANSACTION_STATUS_C
#define TRANSACTION_STATUS_C

const char *transactionStatusToString(enum TransactionStatus status) {
  switch (status) {
  case TS_SAFE:
    return "Aman";
    break;
  case TS_OVER:
    return "Melebihi";
    break;
  default:
    return "Tidak diketahui";
    break;
  }
}

#endif
