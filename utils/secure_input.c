#include "secure_input.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#ifndef SECURE_INPUT_C
#define SECURE_INPUT_C

void disableEcho() {
  struct termios term;
  tcgetattr(STDIN_FILENO, &term);
  term.c_lflag &= ~ECHO;
  tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

void enableEcho() {
  struct termios term;
  tcgetattr(STDIN_FILENO, &term);
  term.c_lflag |= ECHO;
  tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

int readSecurePassword(const char *prompt, char *password, size_t maxLength) {
  if (!prompt || !password || maxLength == 0) {
    return 0;
  }

  printf("%s", prompt);
  fflush(stdout);

  disableEcho();

  size_t index = 0;
  int ch;

  while ((ch = getchar()) != '\n' && ch != EOF) {
    if (ch == 127 || ch == 8) {
      if (index > 0) {
        index--;
        printf("\b \b");
        fflush(stdout);
      }
    } else if (ch >= 32 && ch <= 126) {
      if (index < maxLength - 1) {
        password[index++] = ch;
        printf("*");
        fflush(stdout);
      }
    }
  }

  password[index] = '\0';

  enableEcho();
  printf("\n");

  return (index > 0) ? 1 : 0;
}

int readSecurePasswordWithMask(const char *prompt, char *password,
                               size_t maxLength, char maskChar) {
  if (!prompt || !password || maxLength == 0) {
    return 0;
  }

  printf("%s", prompt);
  fflush(stdout);

  disableEcho();

  size_t index = 0;
  int ch;

  while ((ch = getchar()) != '\n' && ch != EOF) {
    if (ch == 127 || ch == 8) {
      if (index > 0) {
        index--;
        printf("\b \b");
        fflush(stdout);
      }
    } else if (ch >= 32 && ch <= 126) {
      if (index < maxLength - 1) {
        password[index++] = ch;
        printf("%c", maskChar);
        fflush(stdout);
      }
    }
  }

  password[index] = '\0';

  enableEcho();
  printf("\n");

  return (index > 0) ? 1 : 0;
}

int readSilentPassword(const char *prompt, char *password, size_t maxLength) {
  if (!prompt || !password || maxLength == 0) {
    return 0;
  }

  printf("%s", prompt);
  fflush(stdout);

  disableEcho();

  size_t index = 0;
  int ch;

  while ((ch = getchar()) != '\n' && ch != EOF) {
    if (ch == 127 || ch == 8) {
      if (index > 0) {
        index--;
      }
    } else if (ch >= 32 && ch <= 126) {
      if (index < maxLength - 1) {
        password[index++] = ch;
      }
    }
  }

  password[index] = '\0';

  enableEcho();
  printf("\n");

  return (index > 0) ? 1 : 0;
}

int readSecurePasswordWithConfirmation(const char *prompt,
                                       const char *confirmPrompt,
                                       char *password, size_t maxLength) {
  char password1[maxLength];
  char password2[maxLength];

  if (!readSecurePassword(prompt, password1, maxLength)) {
    return 0;
  }

  if (!readSecurePassword(confirmPrompt, password2, maxLength)) {
    return 0;
  }

  if (strcmp(password1, password2) != 0) {
    printf("❌ Password tidak cocok!\n");

    memset(password1, 0, maxLength);
    memset(password2, 0, maxLength);
    return 0;
  }

  strcpy(password, password1);

  memset(password1, 0, maxLength);
  memset(password2, 0, maxLength);

  return 1;
}

int validatePasswordStrength(const char *password) {
  if (!password) {
    return WEAK_PASSWORD;
  }

  size_t length = strlen(password);

  if (length < 6) {
    return WEAK_PASSWORD;
  }

  int hasLower = 0, hasUpper = 0, hasDigit = 0, hasSpecial = 0;

  for (size_t i = 0; i < length; i++) {
    if (islower(password[i]))
      hasLower = 1;
    else if (isupper(password[i]))
      hasUpper = 1;
    else if (isdigit(password[i]))
      hasDigit = 1;
    else if (ispunct(password[i]))
      hasSpecial = 1;
  }

  int score = hasLower + hasUpper + hasDigit + hasSpecial;

  if (length >= 12 && score >= 4) {
    return STRONG_PASSWORD;
  } else if (length >= 8 && score >= 3) {
    return MEDIUM_PASSWORD;
  } else if (length >= 6 && score >= 2) {
    return WEAK_PASSWORD;
  }

  return VERY_WEAK_PASSWORD;
}

const char *getPasswordStrengthDescription(int strength) {
  switch (strength) {
  case VERY_WEAK_PASSWORD:
    return "Sangat Lemah ❌";
  case WEAK_PASSWORD:
    return "Lemah ⚠️";
  case MEDIUM_PASSWORD:
    return "Sedang 🟨";
  case STRONG_PASSWORD:
    return "Kuat ✅";
  default:
    return "Tidak Diketahui";
  }
}

void clearPassword(char *password, size_t length) {
  if (password) {
    memset(password, 0, length);
  }
}

int readSecureUsername(const char *prompt, char *username, size_t maxLength) {
  if (!prompt || !username || maxLength == 0) {
    return 0;
  }

  printf("%s", prompt);
  fflush(stdout);

  if (fgets(username, maxLength, stdin) == NULL) {
    return 0;
  }

  size_t len = strlen(username);
  if (len > 0 && username[len - 1] == '\n') {
    username[len - 1] = '\0';
    len--;
  }

  if (len == 0) {
    return 0;
  }

  for (size_t i = 0; i < len; i++) {
    if (!isalnum(username[i]) && username[i] != '_') {
      printf(
          "❌ Username hanya boleh mengandung huruf, angka, dan underscore!\n");
      return 0;
    }
  }

  return 1;
}

int readSecurePasswordWithStrengthCheck(const char *prompt, char *password,
                                        size_t maxLength) {
  int attempts = 0;
  const int maxAttempts = 3;

  while (attempts < maxAttempts) {
    if (readSecurePassword(prompt, password, maxLength)) {
      int strength = validatePasswordStrength(password);
      printf("💪 Kekuatan Password: %s\n",
             getPasswordStrengthDescription(strength));

      if (strength >= MEDIUM_PASSWORD) {
        return 1;
      } else {
        printf("⚠️ Password terlalu lemah. Gunakan minimal:\n");
        printf("   - 8 karakter\n");
        printf("   - Huruf besar dan kecil\n");
        printf("   - Angka\n");
        printf("   - Simbol khusus\n");

        char choice;
        printf("Tetap gunakan password ini? (y/N): ");
        scanf(" %c", &choice);

        int c;
        while ((c = getchar()) != '\n' && c != EOF)
          ;

        if (choice == 'y' || choice == 'Y') {
          return 1;
        }

        clearPassword(password, maxLength);
        attempts++;

        if (attempts < maxAttempts) {
          printf("Silakan coba password yang lebih kuat:\n");
        }
      }
    } else {
      attempts++;
    }
  }

  printf("❌ Terlalu banyak percobaan yang gagal.\n");
  return 0;
}

#endif
