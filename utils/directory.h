#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

#ifndef DIRECTORY_UTILS_H
#define DIRECTORY_UTILS_H

// Directory creation functions
int createDirectory(const char *path, mode_t mode);
int createDirectoryRecursive(const char *path, mode_t mode);
int directoryExists(const char *path);

// User-specific directory management
int ensureUserDirectoryExists(const char *username);
int ensureUserReportsDirectory(const char *username);
char *getUserDirectoryPath(const char *username);
char *getUserReportsDirectoryPath(const char *username);

// Directory cleanup and validation
int isValidDirectoryName(const char *name);
int removeDirectoryIfEmpty(const char *path);
long long getDirectorySize(const char *path);
int countFilesInDirectory(const char *path);

// Path utilities
char *joinPath(const char *base, const char *relative);
char *normalizePath(const char *path);
int isAbsolutePath(const char *path);

#endif