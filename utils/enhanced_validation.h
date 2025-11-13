#ifndef ENHANCED_VALIDATION_H
#define ENHANCED_VALIDATION_H

#include "../types/config.h"
#include "../types/include.h"
#include <time.h>

int isNullOrEmpty(const char *str);
int isWhitespaceOnly(const char *str);
int containsOnlyDigits(const char *str);
int containsValidCharacters(const char *str, const char *validChars);
int hasMinimumLength(const char *str, int minLength);
int hasMaximumLength(const char *str, int maxLength);

int isValidPositiveNumber(const char *str);
int isValidLongLongRange(const char *str, long long min, long long max);
int isOverflowSafe(const char *str);
int hasDecimalPlaces(const char *str);
int hasLeadingZeros(const char *str);

int isLeapYear(int year);
int getDaysInMonth(int month, int year);
int isValidDay(int day, int month, int year);
int isValidMonth(int month);
int isValidYear(int year);
int isValidDateCombination(int day, int month, int year);

int validateAmountRange(long long amount, long long min, long long max);
int validateBudgetRange(long long budget);
int validateTransactionRange(long long transaction);
int validateMoneyFormat(const char *input);
int validateCurrencyAmount(long long amount);

int validateNameFormat(const char *name);
int validateDescriptionFormat(const char *description);
int validateNameLength(const char *name);
int validateDescriptionLength(const char *description);
int validateStringContent(const char *str);

int validateMenuRange(int choice, int min, int max);
int validatePositiveInteger(int value);
int validateArrayIndex(int index, int arraySize);
int validateListBounds(int index, int listCount);

int isInputBufferOverflow(const char *buffer, size_t maxSize);
int isInputSanitized(const char *input);
int validateUserInput(const char *input, size_t maxLength);
int checkForInjection(const char *input);

long long parseAmountSafely(const char *input);
int parseIntegerSafely(const char *input);
time_t parseDateSafely(const char *input);
int parseChoiceSafely(const char *input, int min, int max);

int validateEdgeCaseAmount(long long amount);
int validateEdgeCaseString(const char *str);
int validateEdgeCaseInteger(int value);
int validateEdgeCaseDate(time_t date);

void sanitizeInput(char *input);
void trimWhitespace(char *str);
void removeExtraSpaces(char *str);
void normalizeInput(char *input);

#endif