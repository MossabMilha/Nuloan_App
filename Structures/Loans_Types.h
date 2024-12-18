//
// Created by PC on 06-Dec-24.
//

#ifndef LOANS_TYPES_H
#define LOANS_TYPES_H
#include <time.h>
typedef struct loan_type{
    int laon_id;
    char loan_title[50];
    char loan_one_line_description[50];
    char loan_description[500];
    float min_amount;
    float max_amount;
    int min_duration;
    int max_duration;
    float interest_rate[2];
    time_t created_at;
    time_t updated_at;
}loan_type;
#endif //LOANS_TYPES_H
