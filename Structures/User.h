#ifndef USER_H
#define USER_H

#include <time.h>

typedef struct User {
    int user_id;
    char role[20];
    char first_name[50];
    char last_name[50];
    char cin[10];
    char email[50];
    char birthday[11];
    unsigned char password_hashed[50];
    time_t created_at;
    time_t updated_at;
    char status[10];
} User;

#endif // USER_H