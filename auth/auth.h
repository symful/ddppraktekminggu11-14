#ifndef AUTH_H
#define AUTH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <time.h>
#include "../types/include.h"

#define USERS_DIR "./users"
#define HASH_FILENAME "password.hash"


extern struct User *currentUser;


int initializeAuthSystem();
int registerUser(const char *username, const char *password);
int loginUser(const char *username, const char *password);
void logoutUser();
int userExists(const char *username);
char *hashPassword(const char *password);
int verifyPassword(const char *password, const char *storedHash);
int createUserDirectory(const char *username);
int savePasswordHash(const char *username, const char *hash);
char *loadPasswordHash(const char *username);
struct User *createUserSession(const char *username);
void destroyUserSession();
char *getUserReportsPath(const char *username);
int createUserReportsDirectory(const char *username);


int isUserAdmin(const char *username);
int createAdminUser(const char *username, const char *password);
struct UserList *getAllUsers();
void freeUserList(struct UserList *userList);
int deleteUser(const char *username);
int changeUserPassword(const char *username, const char *newPassword);
struct User *createAdminUserSession(const char *username);


void saveUserMonthReportToFile(struct MonthReport *report);
void deleteUserMonthReportFile(struct MonthReport *report);
struct MonthReport *loadUserMonthReport(time_t date);
int userReportExists(time_t date);
char *getUserReportsDirectory();
char *getUserConfigPath();
void createDefaultUserConfig();
int initializeUserWorkspace(const char *username);
void cleanupUserSession();


void setUserCategoryBudget(struct MonthReport *report, enum TransactionCategory category, long long budget);
void addUserMonthReportTransaction(struct MonthReport *monthReport, struct Transaction *transaction);
void removeUserMonthReportTransaction(struct MonthReport *monthReport, int groupIndex, int transactionIndex);
void updateUserReportDate(struct MonthReport *report, time_t newDate);
void resetUserBudgets(struct MonthReportList *monthReportList);


void viewAllUserReports();
void showSystemStatistics();


void showAuthMenu();
void showLoginMenu();
void showRegisterMenu();
void showAdminMenu();
void showUserManagementMenu();
int handleAuthMenuChoice();
int handleLogin();
int handleRegister();
int handleAdminLogin();
int handleAdminUserManagement();
int handleUserDeletion();
int handlePasswordReset();
int handleAdminUserCreation();
void clearScreen();

#endif
