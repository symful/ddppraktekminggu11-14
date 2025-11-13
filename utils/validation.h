#include "../types/include.h"
#include "enhanced_validation.h"
#include "input_handler.h"
#include <time.h>

#ifndef VALIDATION_H
#define VALIDATION_H


int validateIntegerInput(const char *input, int *result);
int validateLongLongInput(const char *input, long long *result);
int validateStringInput(const char *input, char *result, size_t maxLength);
int validateMenuChoice(int choice, int minChoice, int maxChoice);
int validateTransactionType(int type);
int validateTransactionCategory(int category);
int validateMonthYearFormat(const char *input, time_t *result);
int validateDateFormat(const char *input, time_t *result);
int validateDescription(const char *description);
int validateTransactionName(const char *name);


int validateBudgetExceeded(struct TransactionGroup *group, long long newAmount);
int validateDuplicateMonthReport(struct MonthReportList *list, time_t date);
int validateReportIndex(struct MonthReportList *list, int index);
int validateGroupIndex(struct MonthReport *report, int index);
int validateTransactionIndex(struct TransactionGroup *group, int index);


const char *getValidationErrorMessage(int errorType);


void trimString(char *str);
int readAndValidateInteger(const char *prompt, int minValue, int maxValue, int *result);
int readAndValidateLongLong(const char *prompt, long long minValue, long long maxValue, long long *result);
int readAndValidateString(const char *prompt, char *result, size_t maxLength);
int readAndValidateDate(const char *prompt, time_t *result);

#endif
