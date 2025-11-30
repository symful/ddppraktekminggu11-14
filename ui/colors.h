#ifndef UI_COLORS_H
#define UI_COLORS_H

#define COLOR_RESET "\033[0m"
#define COLOR_BOLD "\033[1m"
#define COLOR_DIM "\033[2m"
#define COLOR_UNDERLINE "\033[4m"
#define COLOR_BLINK "\033[5m"
#define COLOR_REVERSE "\033[7m"

#define COLOR_BLACK "\033[30m"
#define COLOR_RED "\033[31m"
#define COLOR_GREEN "\033[32m"
#define COLOR_YELLOW "\033[33m"
#define COLOR_BLUE "\033[34m"
#define COLOR_MAGENTA "\033[35m"
#define COLOR_CYAN "\033[36m"
#define COLOR_WHITE "\033[37m"

#define COLOR_BRIGHT_BLACK "\033[90m"
#define COLOR_BRIGHT_RED "\033[91m"
#define COLOR_BRIGHT_GREEN "\033[92m"
#define COLOR_BRIGHT_YELLOW "\033[93m"
#define COLOR_BRIGHT_BLUE "\033[94m"
#define COLOR_BRIGHT_MAGENTA "\033[95m"
#define COLOR_BRIGHT_CYAN "\033[96m"
#define COLOR_BRIGHT_WHITE "\033[97m"

#define BG_BLACK "\033[40m"
#define BG_RED "\033[41m"
#define BG_GREEN "\033[42m"
#define BG_YELLOW "\033[43m"
#define BG_BLUE "\033[44m"
#define BG_MAGENTA "\033[45m"
#define BG_CYAN "\033[46m"
#define BG_WHITE "\033[47m"

#define BG_BRIGHT_BLACK "\033[100m"
#define BG_BRIGHT_RED "\033[101m"
#define BG_BRIGHT_GREEN "\033[102m"
#define BG_BRIGHT_YELLOW "\033[103m"
#define BG_BRIGHT_BLUE "\033[104m"
#define BG_BRIGHT_MAGENTA "\033[105m"
#define BG_BRIGHT_CYAN "\033[106m"
#define BG_BRIGHT_WHITE "\033[107m"

void setColor(const char *color);
void resetColor();
void printColored(const char *text, const char *color);
void printBold(const char *text);
void printColoredBold(const char *text, const char *color);
void printWithBg(const char *text, const char *fgColor, const char *bgColor);
void printSuccess(const char *text);
void printError(const char *text);
void printWarning(const char *text);
void printInfo(const char *text);
const char *getAmountColor(long long amount);
const char *getBudgetStatusColor(double usedPercentage);
void printAmount(long long amount);
void printBudgetStatus(const char *status, double percentage);

#endif