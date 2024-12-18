//
// Created by PC on 05-Dec-24.
//

#include "Eligibility.h"
#include "Structures/LOAN_APPLICATIONS.h"
#include "Structures/User.h"
#include "Structures/LOAN_TERMS.h"
#include <stdio.h>



void get_current_date(char *buffer, size_t size) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    strftime(buffer, size, "%Y-%m-%d %H:%M:%S", t);
}

int calculate_credit_score(User user, LOAN_APPLICATIONS loan_application) {
    int score = 300; // Score initial minimum

    // Critère 1 : Revenus mensuels (pondération forte)
    if (loan_application.income > 10000) {
        score += 200; // Très bons revenus
    } else if (loan_application.income > 5000) {
        score += 100; // Revenus modérés
    } else if (loan_application.income > 3000) {
        score += 50; // Revenus bas mais acceptables
    }

    // Critère 2 : Montant demandé (pondération moyenne)
    if (loan_application.amount_requested < 50000) {
        score += 50; // Montant demandé raisonnable
    } else if (loan_application.amount_requested < 100000) {
        score += 30; // Montant modéré
    } else {
        score -= 50; // Montant élevé, risque accru
    }

    // Critère 3 : Ancienneté du compte utilisateur
    time_t current_time = time(NULL);
    double account_age_years = difftime(current_time, user.created_at) / (365 * 24 * 60 * 60);
    if (account_age_years > 5) {
        score += 100; // Ancienneté élevée
    } else if (account_age_years > 2) {
        score += 50; // Ancienneté modérée
    } else {
        score -= 20; // Compte récent, moins de confiance
    }

    // Critère 4 : Statut utilisateur (bonus si actif)
    if (strcmp(user.status, "active") == 0) {
        score += 50;
    } else {
        score -= 50; // Compte inactif ou douteux
    }

    // Limiter le score entre 300 (minimum) et 850 (maximum)
    if (score > 850) {
        score = 850;
    } else if (score < 300) {
        score = 300;
    }

    return score;
}



void generate_eligibility(User user, LOAN_TERMS loan_terms, LOAN_APPLICATIONS loan_application) {
    Eligibility eligibility = {0};

    // Fill in basic data
    eligibility.user_id = user.user_id;
    eligibility.eligibility_id = loan_application.loan_id;

    // Use loan application data
    eligibility.credit_score = calculate_credit_score(user, loan_application);
    eligibility.income = loan_application.income;
    eligibility.loan_amount_request = loan_application.amount_requested;
    strncpy(eligibility.loan_type, loan_application.loan_type, sizeof(eligibility.loan_type));

    // Calculate application status
    if (eligibility.credit_score >= 650 && eligibility.income >= 3000.0 &&
        eligibility.loan_amount_request >= loan_terms.min_amount &&
        eligibility.loan_amount_request <= loan_terms.max_amount) {
        strncpy(eligibility.status, "Approved", sizeof(eligibility.status));
        eligibility.approved_amount = eligibility.loan_amount_request; // Approve the full requested amount
        } else {
            strncpy(eligibility.status, "Rejected", sizeof(eligibility.status));
            eligibility.approved_amount = 0.0; // Rejected
        }

    // Fill in date fields
    get_current_date(eligibility.created_at, sizeof(eligibility.created_at));
    strncpy(eligibility.updated_at, eligibility.created_at, sizeof(eligibility.updated_at));

    char file_path[200];
    snprintf(file_path, sizeof(file_path), "C:\\Users\\rachi\\CLionProjects\\nuloan\\DataBase\\ELIGIBILITY\\user_%d.bin", eligibility.user_id);

    // Open the file in binary mode
    FILE *file = fopen(file_path, "wb");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    // Write the structure to the file
    if (fwrite(&eligibility, sizeof(Eligibility), 1, file) != 1) {
        perror("Error writing to file");
    } else {
        printf("Data successfully saved to file: %s\n", file_path);
    }

    // Close the file
    fclose(file);
}



