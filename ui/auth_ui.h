#ifndef UI_AUTH_UI_H
#define UI_AUTH_UI_H

void showAuthMenu();
void showLoginMenu();
void showRegisterMenu();
void showAdminLoginForm();
void showAdminMenu();
void showUserManagementMenu();
void showAdminAccessSummary();
int handleLogin();
int handleRegister();
int handleAdminLogin();
int handleAdminUserManagement();
int handleUserDeletion();
int handlePasswordReset();
int handleAdminUserCreation();
int handleAuthMenuChoice();

#endif