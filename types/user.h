#ifndef USER_H
#define USER_H

#define MAX_USERNAME_LENGTH 50
#define MAX_PASSWORD_LENGTH 100
#define HASH_LENGTH 65
#define USERS_DIR "./users"


struct User {
    char username[MAX_USERNAME_LENGTH];
    char userDir[256];
    char hashFile[256];
    int isAdmin;
};


struct UserList {
    char **usernames;
    int count;
};


struct UserSession {
    int isLoggedIn;
    char username[MAX_USERNAME_LENGTH];
    char userDir[256];
    char reportsDir[256];
};

#endif
