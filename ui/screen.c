#ifndef UI_SCREEN_C
#define UI_SCREEN_C

#include "screen.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/ioctl.h>
#include <unistd.h>
#endif

void getTerminalSize(int *width, int *height) {
#ifdef _WIN32
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
    *width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    *height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
  } else {
    *width = DEFAULT_TERM_WIDTH;
    *height = DEFAULT_TERM_HEIGHT;
  }
#else
  struct winsize ws;
  if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == 0 && ws.ws_col > 0 &&
      ws.ws_row > 0) {
    *width = ws.ws_col;
    *height = ws.ws_row;
  } else {
    *width = DEFAULT_TERM_WIDTH;
    *height = DEFAULT_TERM_HEIGHT;
  }
#endif
}

int getTermWidth() {
  int width, height;
  getTerminalSize(&width, &height);
  return width;
}

int getTermHeight() {
  int width, height;
  getTerminalSize(&width, &height);
  return height;
}

int getAdaptiveBoxWidth() {
  int termWidth = getTermWidth();

  if (termWidth < 80) {
    return BOX_WIDTH_SMALL;
  } else if (termWidth < 120) {
    return BOX_WIDTH_MEDIUM;
  } else if (termWidth < 140) {
    return BOX_WIDTH_LARGE;
  }
  return BOX_WIDTH_XLARGE;
}

int getVisibleLength(const char *text) {
  int length = 0;
  bool inEscape = false;

  for (int i = 0; text[i] != '\0'; i++) {
    if (text[i] == '\033') {
      inEscape = true;
    } else if (inEscape && text[i] == 'm') {
      inEscape = false;
    } else if (!inEscape) {
      length++;
    }
  }

  return length;
}

int getHorizontalPadding(int contentWidth) {
  int termWidth = getTermWidth();
  int padding = (termWidth - contentWidth) / 2;
  return padding > 0 ? padding : 0;
}

int getVerticalPadding(int contentHeight) {
  int termHeight = getTermHeight();
  int padding = (termHeight - contentHeight) / 2;

  if (padding < 0) {
    return 1;
  }

  if (contentHeight > termHeight * 0.7) {
    return 2;
  }

  return padding;
}

void printHorizontalPadding(int width) {
  for (int i = 0; i < width; i++) {
    printf(" ");
  }
}

void addVerticalPadding(int lines) {
  for (int i = 0; i < lines; i++) {
    printf("\n");
  }
}

void clearAndCenterVertically(int contentLines) {
#ifdef _WIN32
  system("cls");
#else
  system("clear");
#endif

  int padding = getVerticalPadding(contentLines);
  addVerticalPadding(padding);
}

void printCentered(const char *text, int boxWidth) {
  int termWidth = getTermWidth();

  if (boxWidth > termWidth) {
    printf("%s", text);
    return;
  }

  int padding = (termWidth - boxWidth) / 2;
  printHorizontalPadding(padding);
  printf("%s", text);
}

void printCenteredColored(const char *text, int boxWidth, const char *color) {
  int termWidth = getTermWidth();

  if (boxWidth > termWidth) {
    printf("%s%s\033[0m", color, text);
    return;
  }

  int padding = (termWidth - boxWidth) / 2;
  printHorizontalPadding(padding);
  printf("%s%s\033[0m", color, text);
}

void printCenteredBold(const char *text, int boxWidth) {
  int termWidth = getTermWidth();

  if (boxWidth > termWidth) {
    printf("\033[1m%s\033[0m", text);
    return;
  }

  int padding = (termWidth - boxWidth) / 2;
  printHorizontalPadding(padding);
  printf("\033[1m%s\033[0m", text);
}

void centerText(char *output, const char *text, int width) {
  int textLen = strlen(text);
  int padding = (width - textLen) / 2;

  if (padding <= 0) {
    strcpy(output, text);
    return;
  }

  int idx = 0;
  for (int i = 0; i < padding; i++) {
    output[idx++] = ' ';
  }

  strcpy(output + idx, text);
  idx += textLen;

  int rightPadding = width - textLen - padding;
  for (int i = 0; i < rightPadding; i++) {
    output[idx++] = ' ';
  }

  output[idx] = '\0';
}

#endif
