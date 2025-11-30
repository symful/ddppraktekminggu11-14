#ifndef UI_SCREEN_H
#define UI_SCREEN_H

#define DEFAULT_TERM_WIDTH 120
#define DEFAULT_TERM_HEIGHT 40

#define BOX_WIDTH_SMALL 60
#define BOX_WIDTH_MEDIUM 80
#define BOX_WIDTH_LARGE 100
#define BOX_WIDTH_XLARGE 120

void getTerminalSize(int *width, int *height);
int getTermWidth();
int getTermHeight();
int getAdaptiveBoxWidth();
int getHorizontalPadding(int contentWidth);
int getVerticalPadding(int contentHeight);
void printHorizontalPadding(int width);
void addVerticalPadding(int lines);
void clearAndCenterVertically(int contentLines);
void printCentered(const char *text, int boxWidth);
void printCenteredColored(const char *text, int boxWidth, const char *color);
void printCenteredBold(const char *text, int boxWidth);
int getVisibleLength(const char *text);
void centerText(char *output, const char *text, int width);

#endif