#include "config.h"
#include <stdio.h>
#include <stdlib.h>

#ifndef CONFIG_C
#define CONFIG_C

struct AppConfig globalConfig;

void initializeDefaultConfig() {
  globalConfig.money.minimumAmount = 1;
  globalConfig.money.maximumAmount = 1000000000000LL;
  globalConfig.money.minimumBudget = 1000;
  globalConfig.money.maximumBudget = 10000000000LL;
  globalConfig.money.minimumTransactionAmount = 1;
  globalConfig.money.maximumTransactionAmount = 1000000000LL;

  globalConfig.validation.maxNameLength = 19;
  globalConfig.validation.maxDescriptionLength = 199;
  globalConfig.validation.maxValidationAttempts = 3;
  globalConfig.validation.minYear = 1900;
  globalConfig.validation.maxYear = 2100;
}

void loadConfigFromFile(const char *filename) {
  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    initializeDefaultConfig();
    return;
  }

  char line[256];
  while (fgets(line, sizeof(line), file)) {
    if (sscanf(line, "MIN_AMOUNT=%lld", &globalConfig.money.minimumAmount) == 1)
      continue;
    if (sscanf(line, "MAX_AMOUNT=%lld", &globalConfig.money.maximumAmount) == 1)
      continue;
    if (sscanf(line, "MIN_BUDGET=%lld", &globalConfig.money.minimumBudget) == 1)
      continue;
    if (sscanf(line, "MAX_BUDGET=%lld", &globalConfig.money.maximumBudget) == 1)
      continue;
    if (sscanf(line, "MIN_TRANSACTION=%lld",
               &globalConfig.money.minimumTransactionAmount) == 1)
      continue;
    if (sscanf(line, "MAX_TRANSACTION=%lld",
               &globalConfig.money.maximumTransactionAmount) == 1)
      continue;
    if (sscanf(line, "MAX_NAME_LENGTH=%d",
               &globalConfig.validation.maxNameLength) == 1)
      continue;
    if (sscanf(line, "MAX_DESC_LENGTH=%d",
               &globalConfig.validation.maxDescriptionLength) == 1)
      continue;
    if (sscanf(line, "MAX_ATTEMPTS=%d",
               &globalConfig.validation.maxValidationAttempts) == 1)
      continue;
    if (sscanf(line, "MIN_YEAR=%d", &globalConfig.validation.minYear) == 1)
      continue;
    if (sscanf(line, "MAX_YEAR=%d", &globalConfig.validation.maxYear) == 1)
      continue;
  }

  fclose(file);
}

void saveConfigToFile(const char *filename) {
  FILE *file = fopen(filename, "w");
  if (file == NULL) {
    return;
  }

  fprintf(file, "MIN_AMOUNT=%lld\n", globalConfig.money.minimumAmount);
  fprintf(file, "MAX_AMOUNT=%lld\n", globalConfig.money.maximumAmount);
  fprintf(file, "MIN_BUDGET=%lld\n", globalConfig.money.minimumBudget);
  fprintf(file, "MAX_BUDGET=%lld\n", globalConfig.money.maximumBudget);
  fprintf(file, "MIN_TRANSACTION=%lld\n",
          globalConfig.money.minimumTransactionAmount);
  fprintf(file, "MAX_TRANSACTION=%lld\n",
          globalConfig.money.maximumTransactionAmount);
  fprintf(file, "MAX_NAME_LENGTH=%d\n", globalConfig.validation.maxNameLength);
  fprintf(file, "MAX_DESC_LENGTH=%d\n",
          globalConfig.validation.maxDescriptionLength);
  fprintf(file, "MAX_ATTEMPTS=%d\n",
          globalConfig.validation.maxValidationAttempts);
  fprintf(file, "MIN_YEAR=%d\n", globalConfig.validation.minYear);
  fprintf(file, "MAX_YEAR=%d\n", globalConfig.validation.maxYear);

  fclose(file);
}

int validateMoneyAmount(long long amount) {
  return (amount >= globalConfig.money.minimumAmount &&
          amount <= globalConfig.money.maximumAmount);
}

int validateBudgetAmount(long long amount) {
  return (amount >= globalConfig.money.minimumBudget &&
          amount <= globalConfig.money.maximumBudget);
}

int validateTransactionAmount(long long amount) {
  return (amount >= globalConfig.money.minimumTransactionAmount &&
          amount <= globalConfig.money.maximumTransactionAmount);
}

void setMinimumAmount(long long amount) {
  if (amount > 0 && amount < globalConfig.money.maximumAmount) {
    globalConfig.money.minimumAmount = amount;
  }
}

void setMaximumAmount(long long amount) {
  if (amount > globalConfig.money.minimumAmount) {
    globalConfig.money.maximumAmount = amount;
  }
}

void setMinimumBudget(long long budget) {
  if (budget > 0 && budget < globalConfig.money.maximumBudget) {
    globalConfig.money.minimumBudget = budget;
  }
}

void setMaximumBudget(long long budget) {
  if (budget > globalConfig.money.minimumBudget) {
    globalConfig.money.maximumBudget = budget;
  }
}

long long getMinimumAmount() { return globalConfig.money.minimumAmount; }

long long getMaximumAmount() { return globalConfig.money.maximumAmount; }

long long getMinimumBudget() { return globalConfig.money.minimumBudget; }

long long getMaximumBudget() { return globalConfig.money.maximumBudget; }

#endif
