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
  char *str = (char *)malloc(sizeof(char) * 20);
  struct tm *tm_ptr = localtime(&date);
  strftime(str, 20, "%B %Y", tm_ptr);
  return str;
}

time_t dateFromMMYYYY(char *str) {
  struct tm tm;
  memset(&tm, 0, sizeof(struct tm));

  int month, year;
  if (sscanf(str, "%d/%d", &month, &year) != 2) {
    return -1;
  }

  if (month < 1 || month > 12 || year < 1900 || year > 3000) {
    return -1;
  }

  tm.tm_mon = month - 1;
  tm.tm_year = year - 1900;
  tm.tm_mday = 1;

  return mktime(&tm);
}

#endif
