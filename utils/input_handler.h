#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include "../types/config.h"
#include <time.h>

typedef enum {
    INPUT_SUCCESS = 0,
    INPUT_ERROR_NULL = 1,
    INPUT_ERROR_EMPTY = 2,
    INPUT_ERROR_TOO_LONG = 3,
    INPUT_ERROR_INVALID_FORMAT = 4,
    INPUT_ERROR_OUT_OF_RANGE = 5,
    INPUT_ERROR_ATTEMPTS_EXCEEDED = 6,
    INPUT_ERROR_BUFFER_OVERFLOW = 7,
    INPUT_ERROR_INJECTION_DETECTED = 8
} InputResult;

typedef enum {
    PROMPT_AMOUNT = 0,
    PROMPT_BUDGET = 1,
    PROMPT_TRANSACTION = 2,
    PROMPT_NAME = 3,
    PROMPT_DESCRIPTION = 4,
    PROMPT_DATE = 5,
    PROMPT_CHOICE = 6
} PromptType;

int readRawInput(char *buffer, size_t bufferSize);
void clearInputBuffer();

InputResult promptForAmount(const char *message, long long *result);
InputResult promptForBudget(const char *message, long long *result);
InputResult promptForTransaction(const char *message, long long *result);
InputResult promptForInteger(const char *message, int min, int max, int *result);
InputResult promptForChoice(const char *message, int min, int max, int *result);

InputResult promptForName(const char *message, char *result, size_t maxLength);
InputResult promptForDescription(const char *message, char *result, size_t maxLength);
InputResult promptForString(const char *message, char *result, size_t maxLength);
InputResult promptForDate(const char *message, time_t *result);

int attemptAmountInput(const char *buffer, long long *result);
int attemptBudgetInput(const char *buffer, long long *result);
int attemptTransactionInput(const char *buffer, long long *result);
int attemptIntegerInput(const char *buffer, int min, int max, int *result);
int attemptStringInput(const char *buffer, char *result, size_t maxLength);
int attemptDateInput(const char *buffer, time_t *result);

void displayPrompt(const char *message);
void displayErrorMessage(InputResult error);

void displayRetryMessage(int attempt, int maxAttempts);

int shouldRetryInput(int attempt);
int hasExceededMaxAttempts(int attempt);

InputResult validateInputResult(const char *buffer, PromptType type);
void preprocessInput(char *buffer);

const char* getInputErrorMessage(InputResult error);
int getMaxAttemptsForPrompt(PromptType type);
size_t getMaxLengthForPrompt(PromptType type);

#endif