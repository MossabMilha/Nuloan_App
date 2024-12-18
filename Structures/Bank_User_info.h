//
// Created by PC on 23-Nov-24.
//

#ifndef BANK_USER_INFO_H
#define BANK_USER_INFO_H
#include <time.h>

typedef struct Bank_User_info {
    int bank_id;
    char user_id;
    char bank_name[50];
    char account_type[50];
    char account_holder_name[50];
    char account_number[50];
    time_t created_at;
    time_t updated_at;
}Bank_User_info;
#endif //BANK_USER_INFO_H
