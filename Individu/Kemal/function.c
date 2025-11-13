#include <limits.h>
#include <stdlib.h>
#include <time.h>

int validateIntegerInput(const char *input, int *result) {
  if (input == NULL || result == NULL) {
    return 0;
  }

  char *endptr;
  long temp = strtol(input, &endptr, 10);

  if (*endptr != '\0' && *endptr != '\n') {
    return 0;
  }

  if (temp < INT_MIN || temp > INT_MAX) {
    return 0;
  }

  *result = (int)temp;
  return 1;
}
