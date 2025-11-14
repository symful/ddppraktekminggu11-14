#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef ENV_LOADER_C
#define ENV_LOADER_C

#define MAX_ENV_LINE 256
#define MAX_ENV_KEY 64
#define MAX_ENV_VALUE 192

struct EnvConfig {
  char adminUsername[MAX_ENV_VALUE];
  char adminPassword[MAX_ENV_VALUE];
  long long minAmount;
  long long maxAmount;
  long long minBudget;
  long long maxBudget;
  long long minTransaction;
  long long maxTransaction;
  int maxNameLength;
  int maxDescLength;
  int maxValidationAttempts;
  int minYear;
  int maxYear;
} envConfig;

// Initialize default values
void initializeEnvDefaults() {
  strcpy(envConfig.adminUsername, "admin");
  strcpy(envConfig.adminPassword, "admin123");
  envConfig.minAmount = 1000;
  envConfig.maxAmount = 999999999999LL;
  envConfig.minBudget = 10000;
  envConfig.maxBudget = 999999999999LL;
  envConfig.minTransaction = 500;
  envConfig.maxTransaction = 999999999999LL;
  envConfig.maxNameLength = 50;
  envConfig.maxDescLength = 200;
  envConfig.maxValidationAttempts = 3;
  envConfig.minYear = 2020;
  envConfig.maxYear = 2030;
}

// Parse a single environment line
void parseEnvLine(const char *line) {
  if (line[0] == '#' || line[0] == '\n' || line[0] == '\0') {
    return; // Skip comments and empty lines
  }

  char key[MAX_ENV_KEY];
  char value[MAX_ENV_VALUE];

  // Find the '=' separator
  const char *equals = strchr(line, '=');
  if (!equals) {
    return; // Invalid format
  }

  // Extract key
  size_t keyLen = equals - line;
  if (keyLen >= MAX_ENV_KEY) {
    return; // Key too long
  }
  strncpy(key, line, keyLen);
  key[keyLen] = '\0';

  // Extract value
  strcpy(value, equals + 1);

  // Remove trailing newline
  size_t valueLen = strlen(value);
  if (valueLen > 0 && value[valueLen - 1] == '\n') {
    value[valueLen - 1] = '\0';
  }

  // Set the appropriate config value
  if (strcmp(key, "ADMIN_USERNAME") == 0) {
    strncpy(envConfig.adminUsername, value, MAX_ENV_VALUE - 1);
    envConfig.adminUsername[MAX_ENV_VALUE - 1] = '\0';
  } else if (strcmp(key, "ADMIN_PASSWORD") == 0) {
    strncpy(envConfig.adminPassword, value, MAX_ENV_VALUE - 1);
    envConfig.adminPassword[MAX_ENV_VALUE - 1] = '\0';
  } else if (strcmp(key, "MIN_AMOUNT") == 0) {
    envConfig.minAmount = atoll(value);
  } else if (strcmp(key, "MAX_AMOUNT") == 0) {
    envConfig.maxAmount = atoll(value);
  } else if (strcmp(key, "MIN_BUDGET") == 0) {
    envConfig.minBudget = atoll(value);
  } else if (strcmp(key, "MAX_BUDGET") == 0) {
    envConfig.maxBudget = atoll(value);
  } else if (strcmp(key, "MIN_TRANSACTION") == 0) {
    envConfig.minTransaction = atoll(value);
  } else if (strcmp(key, "MAX_TRANSACTION") == 0) {
    envConfig.maxTransaction = atoll(value);
  } else if (strcmp(key, "MAX_NAME_LENGTH") == 0) {
    envConfig.maxNameLength = atoi(value);
  } else if (strcmp(key, "MAX_DESC_LENGTH") == 0) {
    envConfig.maxDescLength = atoi(value);
  } else if (strcmp(key, "MAX_VALIDATION_ATTEMPTS") == 0) {
    envConfig.maxValidationAttempts = atoi(value);
  } else if (strcmp(key, "MIN_YEAR") == 0) {
    envConfig.minYear = atoi(value);
  } else if (strcmp(key, "MAX_YEAR") == 0) {
    envConfig.maxYear = atoi(value);
  }
}

// Load environment from .env file
int loadEnvFile() {
  initializeEnvDefaults();

  FILE *file = fopen(".env", "r");
  if (!file) {
    printf("File .env tidak ditemukan. Menggunakan konfigurasi default.\n");
    return 0; // Use defaults
  }

  char line[MAX_ENV_LINE];
  while (fgets(line, sizeof(line), file)) {
    parseEnvLine(line);
  }

  fclose(file);
  return 1;
}

// Getter functions for admin credentials
const char *getAdminUsername() { return envConfig.adminUsername; }

const char *getAdminPassword() { return envConfig.adminPassword; }

// Getter functions for financial limits
long long getEnvMinAmount() { return envConfig.minAmount; }

long long getEnvMaxAmount() { return envConfig.maxAmount; }

long long getEnvMinBudget() { return envConfig.minBudget; }

long long getEnvMaxBudget() { return envConfig.maxBudget; }

long long getEnvMinTransaction() { return envConfig.minTransaction; }

long long getEnvMaxTransaction() { return envConfig.maxTransaction; }

// Getter functions for validation settings
int getEnvMaxNameLength() { return envConfig.maxNameLength; }

int getEnvMaxDescLength() { return envConfig.maxDescLength; }

int getEnvMaxValidationAttempts() { return envConfig.maxValidationAttempts; }

int getEnvMinYear() { return envConfig.minYear; }

int getEnvMaxYear() { return envConfig.maxYear; }

#endif
