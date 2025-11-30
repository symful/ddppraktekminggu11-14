#include "../utils/env_loader.c"
#include "../utils/secure_input.c"
#include <string.h>

#ifndef ADMIN_AUTH_C
#define ADMIN_AUTH_C

int initializeAdminAuth() { return loadEnvFile(); }

int validateAdminCredentials(const char *username, const char *password) {
  if (!username || !password) {
    return 0;
  }

  const char *validUsername = getAdminUsername();
  const char *validPassword = getAdminPassword();

  return (strcmp(username, validUsername) == 0 &&
          strcmp(password, validPassword) == 0);
}

#endif
