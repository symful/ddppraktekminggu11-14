#include "../types/include.h"
#include "enhanced_validation.c"
#include "input_handler.c"
#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifndef VALIDATION_C
#define VALIDATION_C

int validateIntegerInput(const char *input, int *result) {
  if (input == NULL || result == NULL) {
    return 0;
  }

  char *endptr;
  long temp = strtol(input, &endptr, 10);

  if (*endptr != '\0' && *endptr != '\n') {
    return 0;
  }

  if (temp < INT_MIN || temp > INT_MAX) {
    return 0;
  }

  *result = (int)temp;
  return 1;
}

int validateLongLongInput(const char *input, long long *result) {
  if (input == NULL || result == NULL) {
    return 0;
  }

  char *endptr;
  long long temp = strtoll(input, &endptr, 10);

  if (*endptr != '\0' && *endptr != '\n') {
    return 0;
  }

  *result = temp;
  return 1;
}

int validateStringInput(const char *input, char *result, size_t maxLength) {
  if (input == NULL || result == NULL || maxLength == 0) {
    return 0;
  }

  const char *start = input;
  while (isspace(*start)) {
    start++;
  }

  if (*start == '\0') {
    return 0;
  }

  size_t len = strlen(start);
  while (len > 0 && isspace(start[len - 1])) {
    len--;
  }

  if (len >= maxLength) {
    return 0;
  }

  strncpy(result, start, len);
  result[len] = '\0';

  return 1;
}

int validateMenuChoice(int choice, int minChoice, int maxChoice) {
  return (choice >= minChoice && choice <= maxChoice);
}

int validateTransactionType(int type) {
  return (type == TT_EXPENSE || type == TT_INCOME);
}

int validateTransactionCategory(int category) {
  return (category >= TC_TRANSPORTATION && category <= TC_OTHER);
}

int validateMonthYearFormat(const char *input, time_t *result) {
  if (input == NULL || result == NULL) {
    return 0;
  }

  int month, year;
  if (sscanf(input, "%d/%d", &month, &year) != 2) {
    return 0;
  }

  if (month < 1 || month > 12) {
    return 0;
  }

  if (year < 1900 || year > 2100) {
    return 0;
  }

  struct tm timeinfo = {0};
  timeinfo.tm_year = year - 1900;
  timeinfo.tm_mon = month - 1;
  timeinfo.tm_mday = 1;

  *result = mktime(&timeinfo);
  return (*result != -1);
}

int validateDateFormat(const char *input, time_t *result) {
  if (input == NULL || result == NULL) {
    return 0;
  }

  int day, month, year;
  if (sscanf(input, "%d/%d/%d", &day, &month, &year) != 3) {
    return 0;
  }

  if (month < 1 || month > 12) {
    return 0;
  }

  if (year < 1900 || year > 2100) {
    return 0;
  }

  int daysInMonth;
  if (month == 2) {

    int isLeap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
    daysInMonth = isLeap ? 29 : 28;
  } else if (month == 4 || month == 6 || month == 9 || month == 11) {
    daysInMonth = 30;
  } else {
    daysInMonth = 31;
  }

  if (day < 1 || day > daysInMonth) {
    return 0;
  }

  struct tm timeinfo = {0};
  timeinfo.tm_year = year - 1900;
  timeinfo.tm_mon = month - 1;
  timeinfo.tm_mday = day;

  *result = mktime(&timeinfo);
  return (*result != -1);
}

int validateDescription(const char *description) {
  if (description == NULL) {
    return 0;
  }

  const char *start = description;
  while (isspace(*start)) {
    start++;
  }

  return (*start != '\0');
}

int validateTransactionName(const char *name) {
  return validateNameFormat(name);
}

int validateBudgetExceeded(struct TransactionGroup *group,
                           long long newAmount) {
  if (group == NULL) {
    return 0;
  }

  return (group->totalRealCost + newAmount > group->maximumCost);
}

int validateDuplicateMonthReport(struct MonthReportList *list, time_t date) {
  if (list == NULL) {
    return 0;
  }

  struct tm *newDate = localtime(&date);
  if (newDate == NULL) {
    return 0;
  }

  for (int i = 0; i < list->amount; i++) {
    struct tm *existingDate = localtime(&list->reports[i]->date);
    if (existingDate != NULL && existingDate->tm_year == newDate->tm_year &&
        existingDate->tm_mon == newDate->tm_mon) {
      return 1;
    }
  }

  return 0;
}

int validateReportIndex(struct MonthReportList *list, int index) {
  if (list == NULL) {
    return 0;
  }

  return (index >= 0 && index < list->amount);
}

int validateGroupIndex(struct MonthReport *report, int index) {
  if (report == NULL) {
    return 0;
  }

  return (index >= 0 && index < report->groupsAmount);
}

int validateTransactionIndex(struct TransactionGroup *group, int index) {
  if (group == NULL) {
    return 0;
  }

  return (index >= 0 && index < group->transactionsAmount);
}

int readAndValidateInteger(const char *prompt, int minValue, int maxValue,
                           int *result) {
  InputResult inputResult =
      promptForInteger(prompt, minValue, maxValue, result);
  return (inputResult == INPUT_SUCCESS) ? 1 : 0;
}

int readAndValidateLongLong(const char *prompt, long long minValue,
                            long long maxValue, long long *result) {
  if (minValue == 1 && maxValue == 1000000000000LL) {
    InputResult inputResult = promptForAmount(prompt, result);
    return (inputResult == INPUT_SUCCESS) ? 1 : 0;
  } else {
    char buffer[32];
    int attempts = 0;
    const int maxAttempts = globalConfig.validation.maxValidationAttempts;

    while (attempts < maxAttempts) {
      printf("%s", prompt);
      if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        buffer[strcspn(buffer, "\n")] = 0;

        long long value;
        if (validateLongLongInput(buffer, &value)) {
          if (value >= minValue && value <= maxValue) {
            *result = value;
            return 1;
          } else {
            printf("Nilai harus antara %lld dan %lld.\n", minValue, maxValue);
          }
        } else {
          printf("Format tidak valid. Harap masukkan angka.\n");
        }
      } else {
        printf("Error membaca input.\n");
      }

      attempts++;
      if (attempts < maxAttempts) {
        printf("Silakan coba lagi (%d/%d).\n", attempts + 1, maxAttempts);
      }
    }

    return 0;
  }
}

int readAndValidateString(const char *prompt, char *result, size_t maxLength) {
  InputResult inputResult = promptForString(prompt, result, maxLength);
  return (inputResult == INPUT_SUCCESS) ? 1 : 0;
}

int readAndValidateDate(const char *prompt, time_t *result) {
  InputResult inputResult = promptForDate(prompt, result);
  return (inputResult == INPUT_SUCCESS) ? 1 : 0;
}

#endif
