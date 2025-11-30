#ifndef UI_COLORS_C
#define UI_COLORS_C

#include "colors.h"
#include <stdbool.h>
#include <stdio.h>

void setColor(const char *color) { printf("%s", color); }

void resetColor() { printf("%s", COLOR_RESET); }

void printColored(const char *text, const char *color) {
  printf("%s%s%s", color, text, COLOR_RESET);
}

void printBold(const char *text) {
  printf("%s%s%s", COLOR_BOLD, text, COLOR_RESET);
}

void printColoredBold(const char *text, const char *color) {
  printf("%s%s%s%s", COLOR_BOLD, color, text, COLOR_RESET);
}

void printWithBg(const char *text, const char *fgColor, const char *bgColor) {
  printf("%s%s%s%s", bgColor, fgColor, text, COLOR_RESET);
}

void printSuccess(const char *text) {
  printf("%s%s%s", COLOR_BRIGHT_GREEN, text, COLOR_RESET);
}

void printError(const char *text) {
  printf("%s%s%s", COLOR_BRIGHT_RED, text, COLOR_RESET);
}

void printWarning(const char *text) {
  printf("%s%s%s", COLOR_BRIGHT_YELLOW, text, COLOR_RESET);
}

void printInfo(const char *text) {
  printf("%s%s%s", COLOR_BRIGHT_BLUE, text, COLOR_RESET);
}

const char *getAmountColor(long long amount) {
  if (amount > 0) {
    return COLOR_BRIGHT_GREEN;
  } else if (amount < 0) {
    return COLOR_BRIGHT_RED;
  }
  return COLOR_WHITE;
}

const char *getBudgetStatusColor(double usedPercentage) {
  if (usedPercentage > 100.0) {
    return COLOR_BRIGHT_RED;
  } else if (usedPercentage >= 80.0) {
    return COLOR_BRIGHT_YELLOW;
  }
  return COLOR_BRIGHT_GREEN;
}

void printAmount(long long amount) {
  const char *color = getAmountColor(amount);
  printf("%s%lld%s", color, amount, COLOR_RESET);
}

void printBudgetStatus(const char *status, double percentage) {
  const char *color = getBudgetStatusColor(percentage);
  printf("%s%s%s", color, status, COLOR_RESET);
}

#endif
