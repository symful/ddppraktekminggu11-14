#include "../types/include.h"
#include <stdlib.h>

#ifndef TRANSACTION_C
#define TRANSACTION_C

struct Transaction *newTransaction() {
  struct Transaction *transaction =
      (struct Transaction *)malloc(sizeof(struct Transaction));
  transaction->amount = 0;
  return transaction;
}

#endif
