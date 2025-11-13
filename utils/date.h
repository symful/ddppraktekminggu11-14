#ifndef DATE_UTILS_H
#define DATE_UTILS_H

#include <time.h>

char *dateToString(time_t date);
char *dateToMonthYearString(time_t date);
time_t dateFromMonthYearString(char *str);
time_t dateFromMMYYYY(char *str);

#endif