#include "input_handler.h"
#include "enhanced_validation.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef INPUT_HANDLER_C
#define INPUT_HANDLER_C

int readRawInput(char *buffer, size_t bufferSize) {
  if (buffer == NULL || bufferSize == 0) {
    return 0;
  }

  if (fgets(buffer, bufferSize, stdin) == NULL) {
    return 0;
  }

  buffer[strcspn(buffer, "\n")] = '\0';
  return 1;
}

void clearInputBuffer() {
  int c;
  while ((c = getchar()) != '\n' && c != EOF) {
  }
}

InputResult promptForAmount(const char *message, long long *result) {
  char buffer[32];
  int attempt = 0;
  int maxAttempts = getMaxAttemptsForPrompt(PROMPT_AMOUNT);

  while (attempt < maxAttempts) {
    displayPrompt(message);

    if (!readRawInput(buffer, sizeof(buffer))) {
      attempt++;
      displayErrorMessage(INPUT_ERROR_BUFFER_OVERFLOW);
      continue;
    }

    preprocessInput(buffer);

    if (attemptAmountInput(buffer, result)) {
      return INPUT_SUCCESS;
    }

    attempt++;
    displayErrorMessage(INPUT_ERROR_INVALID_FORMAT);
    if (shouldRetryInput(attempt)) {
      displayRetryMessage(attempt, maxAttempts);
    }
  }

  return INPUT_ERROR_ATTEMPTS_EXCEEDED;
}

InputResult promptForBudget(const char *message, long long *result) {
  char buffer[32];
  int attempt = 0;
  int maxAttempts = getMaxAttemptsForPrompt(PROMPT_BUDGET);

  while (attempt < maxAttempts) {
    displayPrompt(message);

    if (!readRawInput(buffer, sizeof(buffer))) {
      attempt++;
      displayErrorMessage(INPUT_ERROR_BUFFER_OVERFLOW);
      continue;
    }

    preprocessInput(buffer);

    if (attemptBudgetInput(buffer, result)) {
      return INPUT_SUCCESS;
    }

    attempt++;
    displayErrorMessage(INPUT_ERROR_OUT_OF_RANGE);
    if (shouldRetryInput(attempt)) {
      displayRetryMessage(attempt, maxAttempts);
    }
  }

  return INPUT_ERROR_ATTEMPTS_EXCEEDED;
}

InputResult promptForTransaction(const char *message, long long *result) {
  char buffer[32];
  int attempt = 0;
  int maxAttempts = getMaxAttemptsForPrompt(PROMPT_TRANSACTION);

  while (attempt < maxAttempts) {
    displayPrompt(message);

    if (!readRawInput(buffer, sizeof(buffer))) {
      attempt++;
      displayErrorMessage(INPUT_ERROR_BUFFER_OVERFLOW);
      continue;
    }

    preprocessInput(buffer);

    if (attemptTransactionInput(buffer, result)) {
      return INPUT_SUCCESS;
    }

    attempt++;
    displayErrorMessage(INPUT_ERROR_INVALID_FORMAT);
    if (shouldRetryInput(attempt)) {
      displayRetryMessage(attempt, maxAttempts);
    }
  }

  return INPUT_ERROR_ATTEMPTS_EXCEEDED;
}

InputResult promptForInteger(const char *message, int min, int max,
                             int *result) {
  char buffer[16];
  int attempt = 0;
  int maxAttempts = getMaxAttemptsForPrompt(PROMPT_CHOICE);

  while (attempt < maxAttempts) {
    displayPrompt(message);

    if (!readRawInput(buffer, sizeof(buffer))) {
      attempt++;
      displayErrorMessage(INPUT_ERROR_BUFFER_OVERFLOW);
      continue;
    }

    preprocessInput(buffer);

    if (attemptIntegerInput(buffer, min, max, result)) {
      return INPUT_SUCCESS;
    }

    attempt++;
    displayErrorMessage(INPUT_ERROR_OUT_OF_RANGE);
    if (shouldRetryInput(attempt)) {
      displayRetryMessage(attempt, maxAttempts);
    }
  }

  return INPUT_ERROR_ATTEMPTS_EXCEEDED;
}

InputResult promptForChoice(const char *message, int min, int max,
                            int *result) {
  return promptForInteger(message, min, max, result);
}

InputResult promptForName(const char *message, char *result, size_t maxLength) {
  char buffer[256];
  int attempt = 0;
  int maxAttempts = getMaxAttemptsForPrompt(PROMPT_NAME);

  while (attempt < maxAttempts) {
    displayPrompt(message);

    if (!readRawInput(buffer, sizeof(buffer))) {
      attempt++;
      displayErrorMessage(INPUT_ERROR_BUFFER_OVERFLOW);
      continue;
    }

    preprocessInput(buffer);

    if (attemptStringInput(buffer, result, maxLength)) {
      if (validateNameFormat(result)) {
        return INPUT_SUCCESS;
      }
    }

    attempt++;
    displayErrorMessage(INPUT_ERROR_INVALID_FORMAT);
    if (shouldRetryInput(attempt)) {
      displayRetryMessage(attempt, maxAttempts);
    }
  }

  return INPUT_ERROR_ATTEMPTS_EXCEEDED;
}

InputResult promptForDescription(const char *message, char *result,
                                 size_t maxLength) {
  char buffer[512];
  int attempt = 0;
  int maxAttempts = getMaxAttemptsForPrompt(PROMPT_DESCRIPTION);

  while (attempt < maxAttempts) {
    displayPrompt(message);

    if (!readRawInput(buffer, sizeof(buffer))) {
      attempt++;
      displayErrorMessage(INPUT_ERROR_BUFFER_OVERFLOW);
      continue;
    }

    preprocessInput(buffer);

    if (attemptStringInput(buffer, result, maxLength)) {
      if (validateDescriptionFormat(result)) {
        return INPUT_SUCCESS;
      }
    }

    attempt++;
    displayErrorMessage(INPUT_ERROR_INVALID_FORMAT);
    if (shouldRetryInput(attempt)) {
      displayRetryMessage(attempt, maxAttempts);
    }
  }

  return INPUT_ERROR_ATTEMPTS_EXCEEDED;
}

InputResult promptForString(const char *message, char *result,
                            size_t maxLength) {
  char buffer[512];
  int attempt = 0;
  int maxAttempts = getMaxAttemptsForPrompt(PROMPT_NAME);

  while (attempt < maxAttempts) {
    displayPrompt(message);

    if (!readRawInput(buffer, sizeof(buffer))) {
      attempt++;
      displayErrorMessage(INPUT_ERROR_BUFFER_OVERFLOW);
      continue;
    }

    preprocessInput(buffer);

    if (attemptStringInput(buffer, result, maxLength)) {
      return INPUT_SUCCESS;
    }

    attempt++;
    displayErrorMessage(INPUT_ERROR_INVALID_FORMAT);
    if (shouldRetryInput(attempt)) {
      displayRetryMessage(attempt, maxAttempts);
    }
  }

  return INPUT_ERROR_ATTEMPTS_EXCEEDED;
}

InputResult promptForDate(const char *message, time_t *result) {
  char buffer[16];
  int attempt = 0;
  int maxAttempts = getMaxAttemptsForPrompt(PROMPT_DATE);

  while (attempt < maxAttempts) {
    displayPrompt(message);

    if (!readRawInput(buffer, sizeof(buffer))) {
      attempt++;
      displayErrorMessage(INPUT_ERROR_BUFFER_OVERFLOW);
      continue;
    }

    preprocessInput(buffer);

    if (attemptDateInput(buffer, result)) {
      return INPUT_SUCCESS;
    }

    attempt++;
    displayErrorMessage(INPUT_ERROR_INVALID_FORMAT);
    if (shouldRetryInput(attempt)) {
      displayRetryMessage(attempt, maxAttempts);
    }
  }

  return INPUT_ERROR_ATTEMPTS_EXCEEDED;
}

int attemptAmountInput(const char *buffer, long long *result) {
  if (isNullOrEmpty(buffer)) {
    return 0;
  }

  long long amount = parseAmountSafely(buffer);
  if (amount == -1) {
    return 0;
  }

  if (amount <= 0) {
    return 0;
  }

  if (!validateMoneyAmount(amount)) {
    return 0;
  }

  *result = amount;
  return 1;
}

int attemptBudgetInput(const char *buffer, long long *result) {
  if (isNullOrEmpty(buffer)) {
    return 0;
  }

  long long budget = parseAmountSafely(buffer);
  if (budget == -1) {
    return 0;
  }

  if (budget <= 0) {
    return 0;
  }

  if (!validateBudgetAmount(budget)) {
    return 0;
  }

  *result = budget;
  return 1;
}

int attemptTransactionInput(const char *buffer, long long *result) {
  if (isNullOrEmpty(buffer)) {
    return 0;
  }

  long long transaction = parseAmountSafely(buffer);
  if (transaction == -1) {
    return 0;
  }

  if (transaction <= 0) {
    return 0;
  }

  if (!validateTransactionAmount(transaction)) {
    return 0;
  }

  *result = transaction;
  return 1;
}

int attemptIntegerInput(const char *buffer, int min, int max, int *result) {
  if (isNullOrEmpty(buffer)) {
    return 0;
  }

  int value = parseIntegerSafely(buffer);
  if (value == -1) {
    return 0;
  }

  if (value <= 0) {
    return 0;
  }

  if (!validateMenuRange(value, min, max)) {
    return 0;
  }

  *result = value;
  return 1;
}

int attemptStringInput(const char *buffer, char *result, size_t maxLength) {
  if (isNullOrEmpty(buffer)) {
    return 0;
  }

  if (strlen(buffer) > maxLength) {
    return 0;
  }

  if (!validateStringContent(buffer)) {
    return 0;
  }

  if (strlen(buffer) >= maxLength) {
    return 0;
  }

  strcpy(result, buffer);
  return 1;
}

int attemptDateInput(const char *buffer, time_t *result) {
  if (isNullOrEmpty(buffer)) {
    return 0;
  }

  time_t date = parseDateSafely(buffer);
  if (date == -1) {
    return 0;
  }

  if (date == -1) {
    return 0;
  }

  *result = date;
  return 1;
}

void displayPrompt(const char *message) {
  if (message != NULL) {
    printf("%s", message);
    fflush(stdout);
  }
}

void displayErrorMessage(InputResult error) {
  const char *message = getInputErrorMessage(error);
  printf("❌ %s\n", message);
}

void displayRetryMessage(int attempt, int maxAttempts) {
  printf("⚠️  Silakan coba lagi (%d/%d)\n", attempt + 1, maxAttempts);
}

int shouldRetryInput(int attempt) {
  return attempt < globalConfig.validation.maxValidationAttempts;
}

int hasExceededMaxAttempts(int attempt) {
  return attempt >= globalConfig.validation.maxValidationAttempts;
}

InputResult validateInputResult(const char *buffer, PromptType type) {
  if (isNullOrEmpty(buffer)) {
    return INPUT_ERROR_EMPTY;
  }

  size_t maxLength = getMaxLengthForPrompt(type);
  if (strlen(buffer) > maxLength) {
    return INPUT_ERROR_TOO_LONG;
  }

  return INPUT_SUCCESS;
}

void preprocessInput(char *buffer) {
  if (buffer != NULL) {
    sanitizeInput(buffer);
  }
}

const char *getInputErrorMessage(InputResult error) {
  switch (error) {
  case INPUT_SUCCESS:
    return "Input berhasil";
  case INPUT_ERROR_NULL:
    return "Input tidak boleh kosong";
  case INPUT_ERROR_EMPTY:
    return "Input tidak boleh kosong";
  case INPUT_ERROR_TOO_LONG:
    return "Input terlalu panjang";
  case INPUT_ERROR_INVALID_FORMAT:
    return "Format input tidak valid";
  case INPUT_ERROR_OUT_OF_RANGE:
    return "Nilai di luar rentang yang diizinkan";
  case INPUT_ERROR_ATTEMPTS_EXCEEDED:
    return "Terlalu banyak percobaan input yang gagal";
  case INPUT_ERROR_BUFFER_OVERFLOW:
    return "Buffer input meluap";
  case INPUT_ERROR_INJECTION_DETECTED:
    return "Input berbahaya terdeteksi";
  default:
    return "Kesalahan input tidak dikenal";
  }
}

int getMaxAttemptsForPrompt(PromptType type) {
  return globalConfig.validation.maxValidationAttempts;
}

size_t getMaxLengthForPrompt(PromptType type) {
  switch (type) {
  case PROMPT_NAME:
    return globalConfig.validation.maxNameLength;
  case PROMPT_DESCRIPTION:
    return globalConfig.validation.maxDescriptionLength;
  case PROMPT_AMOUNT:
  case PROMPT_BUDGET:
  case PROMPT_TRANSACTION:
    return 20;
  case PROMPT_DATE:
    return 10;
  case PROMPT_CHOICE:
    return 5;
  default:
    return 100;
  }
}

#endif
