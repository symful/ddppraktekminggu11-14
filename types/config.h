#ifndef CONFIG_H
#define CONFIG_H

struct MoneyConfig {
    long long minimumAmount;
    long long maximumAmount;
    long long minimumBudget;
    long long maximumBudget;
    long long minimumTransactionAmount;
    long long maximumTransactionAmount;
};

struct ValidationConfig {
    int maxNameLength;
    int maxDescriptionLength;
    int maxValidationAttempts;
    int minYear;
    int maxYear;
};

struct AppConfig {
    struct MoneyConfig money;
    struct ValidationConfig validation;
};

extern struct AppConfig globalConfig;

void initializeDefaultConfig();
void loadConfigFromFile(const char *filename);
void saveConfigToFile(const char *filename);
int validateMoneyAmount(long long amount);
int validateBudgetAmount(long long amount);
int validateTransactionAmount(long long amount);
void setMinimumAmount(long long amount);
void setMaximumAmount(long long amount);
void setMinimumBudget(long long budget);
void setMaximumBudget(long long budget);
long long getMinimumAmount();
long long getMaximumAmount();
long long getMinimumBudget();
long long getMaximumBudget();

#endif