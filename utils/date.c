#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#ifndef DATE_UTILS_C
#define DATE_UTILS_C

char *dateToString(time_t date) {
  char *str = (char *)malloc(sizeof(char) * 11);
  strftime(str, 11, "%d/%m/%Y", localtime(&date));
  return str;
}

char *dateToMonthYearString(time_t date) {
  char *str = (char *)malloc(sizeof(char) * 11);
  strftime(str, 11, "%B %Y", localtime(&date));
  return str;
}

time_t dateFromMMYYYY(char *str) {
  struct tm tm;
  memset(&tm, 0, sizeof(struct tm));

  int month, year;
  if (sscanf(str, "%d/%d", &month, &year) != 2) {
    return -1; // Invalid format
  }

  if (month < 1 || month > 12 || year < 1900 || year > 3000) {
    return -1; // Invalid date values
  }

  tm.tm_mon = month - 1;    // tm_mon is 0-based
  tm.tm_year = year - 1900; // tm_year is years since 1900
  tm.tm_mday = 1;           // First day of the month

  return mktime(&tm);
}

#endif
