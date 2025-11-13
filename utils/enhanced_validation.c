#include "enhanced_validation.h"
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifndef ENHANCED_VALIDATION_C
#define ENHANCED_VALIDATION_C

int isNullOrEmpty(const char *str) { return (str == NULL || str[0] == '\0'); }

int isWhitespaceOnly(const char *str) {
  if (isNullOrEmpty(str)) {
    return 1;
  }

  for (int i = 0; str[i] != '\0'; i++) {
    if (!isspace((unsigned char)str[i])) {
      return 0;
    }
  }
  return 1;
}

int containsOnlyDigits(const char *str) {
  if (isNullOrEmpty(str)) {
    return 0;
  }

  for (int i = 0; str[i] != '\0'; i++) {
    if (!isdigit((unsigned char)str[i])) {
      return 0;
    }
  }
  return 1;
}

int containsValidCharacters(const char *str, const char *validChars) {
  if (isNullOrEmpty(str) || validChars == NULL) {
    return 0;
  }

  for (int i = 0; str[i] != '\0'; i++) {
    if (strchr(validChars, str[i]) == NULL) {
      return 0;
    }
  }
  return 1;
}

int hasMinimumLength(const char *str, int minLength) {
  if (isNullOrEmpty(str)) {
    return 0;
  }
  return strlen(str) >= minLength;
}

int hasMaximumLength(const char *str, int maxLength) {
  if (str == NULL) {
    return 0;
  }
  return strlen(str) <= maxLength;
}

int isValidPositiveNumber(const char *str) {
  if (isNullOrEmpty(str)) {
    return 0;
  }

  if (str[0] == '-' || str[0] == '+') {
    return 0;
  }

  return containsOnlyDigits(str);
}

int isValidLongLongRange(const char *str, long long min, long long max) {
  if (!isValidPositiveNumber(str)) {
    return 0;
  }

  char *endptr;
  errno = 0;
  long long value = strtoll(str, &endptr, 10);

  if (errno == ERANGE || *endptr != '\0') {
    return 0;
  }

  return (value >= min && value <= max);
}

int isOverflowSafe(const char *str) {
  if (isNullOrEmpty(str) || strlen(str) > 19) {
    return 0;
  }

  char *endptr;
  errno = 0;
  strtoll(str, &endptr, 10);

  return (errno != ERANGE && *endptr == '\0');
}

int hasDecimalPlaces(const char *str) {
  return (strchr(str, '.') != NULL || strchr(str, ',') != NULL);
}

int hasLeadingZeros(const char *str) {
  if (isNullOrEmpty(str) || strlen(str) < 2) {
    return 0;
  }
  return (str[0] == '0' && isdigit((unsigned char)str[1]));
}

int isLeapYear(int year) {
  return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

int getDaysInMonth(int month, int year) {
  if (month < 1 || month > 12) {
    return 0;
  }

  int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

  if (month == 2 && isLeapYear(year)) {
    return 29;
  }

  return daysInMonth[month - 1];
}

int isValidDay(int day, int month, int year) {
  if (day < 1) {
    return 0;
  }

  int maxDays = getDaysInMonth(month, year);
  return (day <= maxDays);
}

int isValidMonth(int month) { return (month >= 1 && month <= 12); }

int isValidYear(int year) {
  return (year >= globalConfig.validation.minYear &&
          year <= globalConfig.validation.maxYear);
}

int isValidDateCombination(int day, int month, int year) {
  return (isValidYear(year) && isValidMonth(month) &&
          isValidDay(day, month, year));
}

int validateAmountRange(long long amount, long long min, long long max) {
  if (min < 0 || max < 0 || min > max) {
    return 0;
  }
  return (amount >= min && amount <= max);
}

int validateBudgetRange(long long budget) {
  if (budget < globalConfig.money.minimumBudget) {
    return 0;
  }
  if (budget > globalConfig.money.maximumBudget) {
    return 0;
  }
  return 1;
}

int validateTransactionRange(long long transaction) {
  if (transaction < globalConfig.money.minimumTransactionAmount) {
    return 0;
  }
  if (transaction > globalConfig.money.maximumTransactionAmount) {
    return 0;
  }
  return 1;
}

int validateMoneyFormat(const char *input) {
  if (isNullOrEmpty(input)) {
    return 0;
  }

  if (hasDecimalPlaces(input)) {
    return 0;
  }

  if (hasLeadingZeros(input)) {
    return 0;
  }

  return isValidPositiveNumber(input);
}

int validateCurrencyAmount(long long amount) {
  if (amount < 1) {
    return 0;
  }

  if (amount > globalConfig.money.maximumAmount) {
    return 0;
  }

  return 1;
}

int validateNameFormat(const char *name) {
  if (isNullOrEmpty(name)) {
    return 0;
  }

  if (isWhitespaceOnly(name)) {
    return 0;
  }

  return validateNameLength(name);
}

int validateDescriptionFormat(const char *description) {
  if (isNullOrEmpty(description)) {
    return 0;
  }

  if (isWhitespaceOnly(description)) {
    return 0;
  }

  return validateDescriptionLength(description);
}

int validateNameLength(const char *name) {
  if (isNullOrEmpty(name)) {
    return 0;
  }

  size_t len = strlen(name);
  return (len >= 1 && len <= globalConfig.validation.maxNameLength);
}

int validateDescriptionLength(const char *description) {
  if (isNullOrEmpty(description)) {
    return 0;
  }

  size_t len = strlen(description);
  return (len >= 1 && len <= globalConfig.validation.maxDescriptionLength);
}

int validateStringContent(const char *str) {
  if (isNullOrEmpty(str)) {
    return 0;
  }

  for (int i = 0; str[i] != '\0'; i++) {
    unsigned char c = (unsigned char)str[i];
    if (!isalnum(c) && !isspace(c) && c != '_' && c != '-' && c != '.') {
      return 0;
    }
  }

  return 1;
}

int validateMenuRange(int choice, int min, int max) {
  if (min < 0 || max < min) {
    return 0;
  }
  return (choice >= min && choice <= max);
}

int validatePositiveInteger(int value) { return (value > 0); }

int validateArrayIndex(int index, int arraySize) {
  return (index >= 0 && index < arraySize);
}

int validateListBounds(int index, int listCount) {
  return (index >= 0 && index < listCount);
}

int isInputBufferOverflow(const char *buffer, size_t maxSize) {
  if (buffer == NULL || maxSize == 0) {
    return 1;
  }

  return (strlen(buffer) >= maxSize);
}

int isInputSanitized(const char *input) {
  if (isNullOrEmpty(input)) {
    return 0;
  }

  return validateStringContent(input);
}

int validateUserInput(const char *input, size_t maxLength) {
  if (isNullOrEmpty(input)) {
    return 0;
  }

  if (strlen(input) > maxLength) {
    return 0;
  }

  return isInputSanitized(input);
}

int checkForInjection(const char *input) {
  if (isNullOrEmpty(input)) {
    return 0;
  }

  const char *dangerous[] = {";", "&&", "||", "|",   ">",      "<",    "&",
                             "$", "`",  "rm", "del", "format", "exit", "quit"};
  int dangerousCount = sizeof(dangerous) / sizeof(dangerous[0]);

  for (int i = 0; i < dangerousCount; i++) {
    if (strstr(input, dangerous[i]) != NULL) {
      return 1;
    }
  }

  return 0;
}

long long parseAmountSafely(const char *input) {
  if (!validateMoneyFormat(input)) {
    return -1;
  }

  if (!isOverflowSafe(input)) {
    return -1;
  }

  char *endptr;
  errno = 0;
  long long result = strtoll(input, &endptr, 10);

  if (errno == ERANGE || *endptr != '\0') {
    return -1;
  }

  return result;
}

int parseIntegerSafely(const char *input) {
  if (isNullOrEmpty(input)) {
    return -1;
  }

  if (!containsOnlyDigits(input)) {
    return -1;
  }

  char *endptr;
  errno = 0;
  long result = strtol(input, &endptr, 10);

  if (errno == ERANGE || *endptr != '\0') {
    return -1;
  }

  if (result < INT_MIN || result > INT_MAX) {
    return -1;
  }

  return (int)result;
}

time_t parseDateSafely(const char *input) {
  if (isNullOrEmpty(input)) {
    return -1;
  }

  int day, month, year;
  if (sscanf(input, "%d/%d/%d", &day, &month, &year) != 3) {
    return -1;
  }

  if (!isValidDateCombination(day, month, year)) {
    return -1;
  }

  struct tm timeinfo = {0};
  timeinfo.tm_year = year - 1900;
  timeinfo.tm_mon = month - 1;
  timeinfo.tm_mday = day;

  time_t result = mktime(&timeinfo);
  return result;
}

int parseChoiceSafely(const char *input, int min, int max) {
  int choice = parseIntegerSafely(input);
  if (choice == -1) {
    return -1;
  }

  if (!validateMenuRange(choice, min, max)) {
    return -1;
  }

  return choice;
}

int validateEdgeCaseAmount(long long amount) {
  if (amount == 0) {
    return 0;
  }

  if (amount < 0) {
    return 0;
  }

  if (amount == LLONG_MAX) {
    return 0;
  }

  return validateCurrencyAmount(amount);
}

int validateEdgeCaseString(const char *str) {
  if (str == NULL) {
    return 0;
  }

  if (strlen(str) == 0) {
    return 0;
  }

  if (checkForInjection(str)) {
    return 0;
  }

  return validateStringContent(str);
}

int validateEdgeCaseInteger(int value) {
  if (value == INT_MIN || value == INT_MAX) {
    return 0;
  }

  return 1;
}

int validateEdgeCaseDate(time_t date) {
  if (date == -1) {
    return 0;
  }

  if (date == 0) {
    return 0;
  }

  struct tm *timeinfo = localtime(&date);
  if (timeinfo == NULL) {
    return 0;
  }

  return isValidDateCombination(timeinfo->tm_mday, timeinfo->tm_mon + 1,
                                timeinfo->tm_year + 1900);
}

void sanitizeInput(char *input) {
  if (input == NULL) {
    return;
  }

  trimWhitespace(input);
  removeExtraSpaces(input);
  normalizeInput(input);
}

void trimWhitespace(char *str) {
  if (str == NULL) {
    return;
  }

  char *start = str;
  while (isspace((unsigned char)*start)) {
    start++;
  }

  if (start != str) {
    memmove(str, start, strlen(start) + 1);
  }

  size_t len = strlen(str);
  while (len > 0 && isspace((unsigned char)str[len - 1])) {
    str[--len] = '\0';
  }
}

void removeExtraSpaces(char *str) {
  if (str == NULL) {
    return;
  }

  char *src = str;
  char *dst = str;
  int spaceFound = 0;

  while (*src != '\0') {
    if (isspace((unsigned char)*src)) {
      if (!spaceFound) {
        *dst = ' ';
        dst++;
        spaceFound = 1;
      }
    } else {
      *dst = *src;
      dst++;
      spaceFound = 0;
    }
    src++;
  }

  *dst = '\0';
}

void normalizeInput(char *input) {
  if (input == NULL) {
    return;
  }

  for (int i = 0; input[i] != '\0'; i++) {
    if (input[i] >= 'A' && input[i] <= 'Z') {
      input[i] = input[i] + ('a' - 'A');
    }
  }
}

#endif
