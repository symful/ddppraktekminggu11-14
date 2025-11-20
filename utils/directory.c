#include "directory.h"
#include "../types/user.h"
#include <dirent.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#ifndef DIRECTORY_UTILS_C
#define DIRECTORY_UTILS_C

// ================================
// Basic Directory Operations
// ================================

int createDirectory(const char *path, mode_t mode) {
  if (path == NULL) {
    return -1;
  }

  struct stat st = {0};

  // Check if directory already exists
  if (stat(path, &st) == 0) {
    if (S_ISDIR(st.st_mode)) {
      return 0; // Directory already exists
    } else {
      errno = ENOTDIR;
      return -1; // Path exists but is not a directory
    }
  }

  // Create the directory
  if (mkdir(path, mode) == 0) {
    return 0;
  }

  return -1;
}

int createDirectoryRecursive(const char *path, mode_t mode) {
  if (path == NULL) {
    return -1;
  }

  char *pathCopy = (char *)malloc(strlen(path) + 1);
  if (pathCopy == NULL) {
    return -1;
  }
  strcpy(pathCopy, path);

  char *current = pathCopy;
  char *slash = current;

  // Skip leading slash for absolute paths
  if (*current == '/') {
    current++;
    slash = current;
  }

  // Create directories one by one
  while ((slash = strchr(current, '/')) != NULL) {
    *slash = '\0';

    if (strlen(pathCopy) > 0 && createDirectory(pathCopy, mode) != 0 &&
        errno != EEXIST) {
      free(pathCopy);
      return -1;
    }

    *slash = '/';
    current = slash + 1;
  }

  // Create the final directory
  int result = createDirectory(pathCopy, mode);
  free(pathCopy);

  return result;
}

int directoryExists(const char *path) {
  if (path == NULL) {
    return 0;
  }

  struct stat st;
  return (stat(path, &st) == 0 && S_ISDIR(st.st_mode));
}

// ================================
// User-specific Directory Management
// ================================

int ensureUserDirectoryExists(const char *username) {
  if (username == NULL || strlen(username) == 0) {
    return 0;
  }

  if (!isValidDirectoryName(username)) {
    fprintf(stderr, "Invalid username for directory: %s\n", username);
    return 0;
  }

  char userPath[512];
  snprintf(userPath, sizeof(userPath), "%s/%s", USERS_DIR, username);

  if (createDirectoryRecursive(userPath, 0700) != 0) {
    perror("Failed to create user directory");
    return 0;
  }

  return 1;
}

int ensureUserReportsDirectory(const char *username) {
  if (username == NULL || strlen(username) == 0) {
    return 0;
  }

  // First ensure user directory exists
  if (!ensureUserDirectoryExists(username)) {
    return 0;
  }

  char reportsPath[512];
  snprintf(reportsPath, sizeof(reportsPath), "%s/%s/reports", USERS_DIR,
           username);

  if (createDirectoryRecursive(reportsPath, 0700) != 0) {
    perror("Failed to create user reports directory");
    return 0;
  }

  return 1;
}

char *getUserDirectoryPath(const char *username) {
  if (username == NULL || strlen(username) == 0) {
    return NULL;
  }

  char *path = (char *)malloc(512);
  if (path == NULL) {
    return NULL;
  }

  snprintf(path, 512, "%s/%s", USERS_DIR, username);
  return path;
}

char *getUserReportsDirectoryPath(const char *username) {
  if (username == NULL || strlen(username) == 0) {
    return NULL;
  }

  char *path = (char *)malloc(512);
  if (path == NULL) {
    return NULL;
  }

  snprintf(path, 512, "%s/%s/reports", USERS_DIR, username);
  return path;
}

// ================================
// Directory Cleanup and Validation
// ================================

int isValidDirectoryName(const char *name) {
  if (name == NULL || strlen(name) == 0) {
    return 0;
  }

  // Check length
  if (strlen(name) > MAX_USERNAME_LENGTH - 1) {
    return 0;
  }

  // Check for invalid characters
  for (int i = 0; name[i] != '\0'; i++) {
    char c = name[i];

    // Allow alphanumeric characters, underscore, and hyphen
    if (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||
          (c >= '0' && c <= '9') || c == '_' || c == '-')) {
      return 0;
    }
  }

  // Check for reserved names
  if (strcmp(name, ".") == 0 || strcmp(name, "..") == 0 ||
      strcmp(name, "admin") == 0 || strcmp(name, "root") == 0) {
    return 0;
  }

  return 1;
}

int removeDirectoryIfEmpty(const char *path) {
  if (path == NULL) {
    return 0;
  }

  DIR *dir = opendir(path);
  if (dir == NULL) {
    return 0;
  }

  struct dirent *entry;
  int isEmpty = 1;

  // Check if directory is empty (only contains . and ..)
  while ((entry = readdir(dir)) != NULL) {
    if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
      isEmpty = 0;
      break;
    }
  }

  closedir(dir);

  if (isEmpty) {
    return (rmdir(path) == 0);
  }

  return 0; // Directory not empty
}

long long getDirectorySize(const char *path) {
  if (path == NULL) {
    return -1;
  }

  DIR *dir = opendir(path);
  if (dir == NULL) {
    return -1;
  }

  struct dirent *entry;
  long long totalSize = 0;
  char fullPath[1024];

  while ((entry = readdir(dir)) != NULL) {
    if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
      continue;
    }

    snprintf(fullPath, sizeof(fullPath), "%s/%s", path, entry->d_name);

    struct stat st;
    if (stat(fullPath, &st) == 0) {
      if (S_ISREG(st.st_mode)) {
        totalSize += st.st_size;
      } else if (S_ISDIR(st.st_mode)) {
        long long subdirSize = getDirectorySize(fullPath);
        if (subdirSize >= 0) {
          totalSize += subdirSize;
        }
      }
    }
  }

  closedir(dir);
  return totalSize;
}

int countFilesInDirectory(const char *path) {
  if (path == NULL) {
    return -1;
  }

  DIR *dir = opendir(path);
  if (dir == NULL) {
    return -1;
  }

  struct dirent *entry;
  int fileCount = 0;
  char fullPath[1024];

  while ((entry = readdir(dir)) != NULL) {
    if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
      continue;
    }

    snprintf(fullPath, sizeof(fullPath), "%s/%s", path, entry->d_name);

    struct stat st;
    if (stat(fullPath, &st) == 0 && S_ISREG(st.st_mode)) {
      fileCount++;
    }
  }

  closedir(dir);
  return fileCount;
}

// ================================
// Path Utilities
// ================================

char *joinPath(const char *base, const char *relative) {
  if (base == NULL || relative == NULL) {
    return NULL;
  }

  size_t baseLen = strlen(base);
  size_t relativeLen = strlen(relative);
  size_t totalLen = baseLen + relativeLen + 2; // +2 for '/' and '\0'

  char *result = (char *)malloc(totalLen);
  if (result == NULL) {
    return NULL;
  }

  strcpy(result, base);

  // Add separator if needed
  if (baseLen > 0 && base[baseLen - 1] != '/' && relative[0] != '/') {
    strcat(result, "/");
  } else if (baseLen > 0 && base[baseLen - 1] == '/' && relative[0] == '/') {
    // Remove duplicate slash
    relative++; // Skip the leading slash in relative
  }

  strcat(result, relative);

  return result;
}

char *normalizePath(const char *path) {
  if (path == NULL) {
    return NULL;
  }

  char *normalized = (char *)malloc(strlen(path) + 1);
  if (normalized == NULL) {
    return NULL;
  }

  strcpy(normalized, path);

  // Remove trailing slashes (except for root)
  size_t len = strlen(normalized);
  while (len > 1 && normalized[len - 1] == '/') {
    normalized[len - 1] = '\0';
    len--;
  }

  // Replace multiple consecutive slashes with single slash
  char *read = normalized;
  char *write = normalized;
  int prevWasSlash = 0;

  while (*read) {
    if (*read == '/') {
      if (!prevWasSlash) {
        *write++ = *read;
        prevWasSlash = 1;
      }
    } else {
      *write++ = *read;
      prevWasSlash = 0;
    }
    read++;
  }
  *write = '\0';

  return normalized;
}

int isAbsolutePath(const char *path) {
  return (path != NULL && path[0] == '/');
}

#endif
