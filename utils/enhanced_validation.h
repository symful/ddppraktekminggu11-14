#ifndef ENHANCED_VALIDATION_H
#define ENHANCED_VALIDATION_H

#include "../types/config.h"
#include "../types/include.h"
#include <time.h>

int isNullOrEmpty(const char *str);
int isWhitespaceOnly(const char *str);
int containsOnlyDigits(const char *str);

int isValidPositiveNumber(const char *str);


int isLeapYear(int year);
int getDaysInMonth(int month, int year);
int isValidDay(int day, int month, int year);
int isValidMonth(int month);
int isValidYear(int year);
int isValidDateCombination(int day, int month, int year);



int validateNameFormat(const char *name);
int validateDescriptionFormat(const char *description);
int validateNameLength(const char *name);
int validateDescriptionLength(const char *description);
int validateStringContent(const char *str);

int validateMenuRange(int choice, int min, int max);





long long parseAmountSafely(const char *input);
int parseIntegerSafely(const char *input);
time_t parseDateSafely(const char *input);
int parseChoiceSafely(const char *input, int min, int max);



void sanitizeInput(char *input);
void trimWhitespace(char *str);

#endif