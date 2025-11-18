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

int isValidPositiveNumber(const char *str) {
  if (isNullOrEmpty(str)) {
    return 0;
  }

  if (str[0] == '-') {
    return 0;
  }

  if (str[0] == '+') {
    str++;
  }

  return containsOnlyDigits(str);
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
    if (!isalnum(c) && !isspace(c) && c != '_' && c != '-' && c != '.' &&
        c != '/') {
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

long long parseAmountSafely(const char *input) {
  if (!isValidPositiveNumber(input)) {
    return -1;
  }

  if (isNullOrEmpty(input) || strlen(input) > 19) {
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

void sanitizeInput(char *input) {
  if (input == NULL) {
    return;
  }

  trimWhitespace(input);
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

#endif
