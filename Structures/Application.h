//
// Created by PC on 29-Nov-24.
//

#ifndef LOAN_APPLICATIONS_H
#define LOAN_APPLICATIONS_H
#include <time.h>
typedef struct Application {
    int loan_application_id;
    int user_id;
    int loan_id;
    float income;
    float amount_requested;
    char application_status[50];  // "Approved", "Declined", or "In Progress"
    char application_date[11];
    float interest_rate;
    time_t repayment_start_date;
    int loan_duration;
    float total_repayment;
    time_t created_at;
    time_t updated_at;
} Application;

#endif //LOAN_APPLICATIONS_H
