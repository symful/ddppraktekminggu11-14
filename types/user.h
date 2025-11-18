#ifndef USER_H
#define USER_H

#define MAX_USERNAME_LENGTH 50
#define MAX_PASSWORD_LENGTH 100
#define HASH_LENGTH 65
#define USERS_DIR "./users"

// User structure
struct User {
    char username[MAX_USERNAME_LENGTH];
    char userDir[256];
    char hashFile[256];
    int isAdmin;
};

// User list structure
struct UserList {
    char **usernames;
    int count;
};

// User session structure
struct UserSession {
    int isLoggedIn;
    char username[MAX_USERNAME_LENGTH];
    char userDir[256];
    char reportsDir[256];
};

#endif // USER_H