//
// Created by PC on 29-Nov-24.
//

#ifndef ROLES_H
#define ROLES_H

#include <time.h>
typedef struct Roles {
    int role_id;
    char role_name[50];
    char role_description[50];
    time_t created_at;
    time_t updated_at;
}Roles;
#endif //ROLES_H
