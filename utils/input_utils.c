#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifndef INPUT_UTILS_C
#define INPUT_UTILS_C

// Clear input buffer
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Read a line from stdin
int readLine(char *buffer, size_t maxLen) {
    if (!buffer || maxLen == 0) {
        return 0;
    }

    if (fgets(buffer, maxLen, stdin) == NULL) {
        return 0;
    }

    // Remove trailing newline
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }

    return 1;
}

// Read integer with validation
int readInteger(const char *prompt, int *result) {
    char buffer[50];
    printf("%s", prompt);

    if (!readLine(buffer, sizeof(buffer))) {
        return 0;
    }

    char *endptr;
    long value = strtol(buffer, &endptr, 10);

    // Check if conversion was successful
    if (*endptr != '\0' || endptr == buffer) {
        printf("Input tidak valid. Harap masukkan angka yang benar.\n");
        return 0;
    }

    *result = (int)value;
    return 1;
}

// Read long long with validation
int readLongLong(const char *prompt, long long *result) {
    char buffer[50];
    printf("%s", prompt);

    if (!readLine(buffer, sizeof(buffer))) {
        return 0;
    }

    char *endptr;
    long long value = strtoll(buffer, &endptr, 10);

    // Check if conversion was successful
    if (*endptr != '\0' || endptr == buffer) {
        printf("Input tidak valid. Harap masukkan angka yang benar.\n");
        return 0;
    }

    *result = value;
    return 1;
}

// Read integer within range
int readIntegerRange(const char *prompt, int min, int max, int *result) {
    int value;
    int attempts = 0;
    const int maxAttempts = 3;

    while (attempts < maxAttempts) {
        if (readInteger(prompt, &value)) {
            if (value >= min && value <= max) {
                *result = value;
                return 1;
            } else {
                printf("Input harus antara %d dan %d.\n", min, max);
            }
        }
        attempts++;
        if (attempts < maxAttempts) {
            printf("Sisa percobaan: %d\n", maxAttempts - attempts);
        }
    }

    printf("Terlalu banyak percobaan yang salah.\n");
    return 0;
}

// Read long long within range
int readLongLongRange(const char *prompt, long long min, long long max, long long *result) {
    long long value;
    int attempts = 0;
    const int maxAttempts = 3;

    while (attempts < maxAttempts) {
        if (readLongLong(prompt, &value)) {
            if (value >= min && value <= max) {
                *result = value;
                return 1;
            } else {
                printf("Input harus antara %lld dan %lld.\n", min, max);
            }
        }
        attempts++;
        if (attempts < maxAttempts) {
            printf("Sisa percobaan: %d\n", maxAttempts - attempts);
        }
    }

    printf("Terlalu banyak percobaan yang salah.\n");
    return 0;
}

// Read string with validation
int readString(const char *prompt, char *buffer, size_t maxLen) {
    if (!buffer || maxLen == 0) {
        return 0;
    }

    printf("%s", prompt);

    if (!readLine(buffer, maxLen)) {
        printf("Error membaca input.\n");
        return 0;
    }

    // Check if string is not empty
    if (strlen(buffer) == 0) {
        printf("Input tidak boleh kosong.\n");
        return 0;
    }

    return 1;
}

// Read string with length validation
int readStringLength(const char *prompt, char *buffer, size_t maxLen, int minLength) {
    if (!readString(prompt, buffer, maxLen)) {
        return 0;
    }

    if (strlen(buffer) < minLength) {
        printf("Input minimal %d karakter.\n", minLength);
        return 0;
    }

    return 1;
}

// Read yes/no confirmation
int readConfirmation(const char *prompt) {
    char buffer[10];
    printf("%s (y/n): ", prompt);

    if (!readLine(buffer, sizeof(buffer))) {
        return 0;
    }

    // Convert to lowercase for comparison
    for (int i = 0; buffer[i]; i++) {
        buffer[i] = tolower(buffer[i]);
    }

    if (strcmp(buffer, "y") == 0 || strcmp(buffer, "yes") == 0 ||
        strcmp(buffer, "ya") == 0 || strcmp(buffer, "iya") == 0) {
        return 1;
    } else if (strcmp(buffer, "n") == 0 || strcmp(buffer, "no") == 0 ||
               strcmp(buffer, "tidak") == 0) {
        return 0;
    } else {
        printf("Harap jawab dengan 'y' atau 'n'.\n");
        return -1; // Invalid input
    }
}

// Wait for Enter key
void waitForEnter() {
    printf("\nTekan Enter untuk melanjutkan...");
    clearInputBuffer();
}

// Show error message and wait
void showError(const char *message) {
    printf("\n❌ Error: %s\n", message);
    waitForEnter();
}

// Show success message and wait
void showSuccess(const char *message) {
    printf("\n✅ %s\n", message);
    waitForEnter();
}

// Show info message and wait
void showInfo(const char *message) {
    printf("\n💡 %s\n", message);
    waitForEnter();
}

#endif
