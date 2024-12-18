// Updated code to log the specific file that fails to open
#include "Auto_Check_application.h"
#include <errno.h>
#include "Send_Email.h"
int count_files_in_directory_2(const char *directory_path) {
    int file_count = 0;
    struct dirent *entry;
    DIR *dp = opendir(directory_path);

    if (dp == NULL) {
        perror("Failed to open directory");
        return 0;
    }

    while ((entry = readdir(dp))) {
        struct stat path_stat;
        char full_path[1024];
        snprintf(full_path, sizeof(full_path), "%s/%s", directory_path, entry->d_name);
        if (stat(full_path, &path_stat) == 0 && S_ISREG(path_stat.st_mode)) {
            file_count++;
        }
    }

    closedir(dp);
    return file_count;
}

void update_application_status(const char *file_name, int target_id, const char *new_status) {
    FILE *file = fopen(file_name, "rb+");
    if (!file) {
        fprintf(stderr, "Error opening file '%s': %s\n", file_name, strerror(errno));
        return;
    }

    Application app;
    while (fread(&app, sizeof(Application), 1, file) == 1) {
        if (app.loan_application_id == target_id) {
            strncpy(app.application_status, new_status, sizeof(app.application_status) - 1);
            app.application_status[sizeof(app.application_status) - 1] = '\0';
            app.updated_at = time(NULL);
            fseek(file, -(long)sizeof(Application), SEEK_CUR);
            fwrite(&app, sizeof(Application), 1, file);
            fclose(file);
            return;
        }
    }

    fclose(file);
}

void delete_id_from_file(const char *file_name, int id_to_delete) {
    FILE *file = fopen(file_name, "r");
    if (!file) {
        fprintf(stderr, "Error opening file '%s': %s\n", file_name, strerror(errno));
        return;
    }

    int ids[1000];
    int count = 0;
    while (fscanf(file, "%d", &ids[count]) == 1) {
        count++;
    }
    fclose(file);

    file = fopen(file_name, "w");
    if (!file) {
        fprintf(stderr, "Error opening file '%s' for writing: %s\n", file_name, strerror(errno));
        return;
    }

    for (int i = 0; i < count; i++) {
        if (ids[i] != id_to_delete) {
            fprintf(file, "%d\n", ids[i]);
        }
    }

    fclose(file);
}
void send_application_declined_email(int user_id) {
    // Retrieve user data (name and email) based on user_id
    char filename[100];
    sprintf(filename, "..\\DataBase\\Users\\user_%d.bin", user_id);

    FILE *file = fopen(filename, "rb+");
    if (file == NULL) {
        fprintf(stderr, "Error: Could not open file for user ID %d.\n", user_id);
        return;
    }

    User user;
    if (fread(&user, sizeof(User), 1, file) != 1) {
        fprintf(stderr, "Error: Could not read user data for user ID %d.\n", user_id);
        fclose(file);
        return;
    }

    char message[1024];

    // Combine first and last name into a full name
    char full_name[200];
    snprintf(full_name, sizeof(full_name), "%s %s", user.first_name, user.last_name);

    // Create the message informing the user that their application was declined
    snprintf(message, sizeof(message),
         "Dear %s,\n\n"
         "We regret to inform you that your application for a NULOAN account has been declined.\n\n"
         "After reviewing your application, we are unable to proceed with your request at this time.\n\n"
         "If you have any questions or believe there has been an error, please feel free to contact us for clarification.\n\n"
         "Best regards,\n"
         "The NULOAN Team", full_name);

    send_email(user.email, "Loan Application Status", message);  // Assuming you have a send_email function
}

void Auto_Check_app() {
    printf("Auto_Check_app\n");
    sir_ltema:
        int max = count_files_in_directory_2("..\\DataBase\\Application_Not_Checked");
    for (int i = 1; i <= max; i++) {
        FILE *id_file = fopen("..\\DataBase\\Id_Of_applications.txt", "r");
        if (id_file == NULL) {
            fprintf(stderr, "Failed to open ID file: %s\n", strerror(errno));
            return;
        }

        int id;
        int current_line = 0;
        while (fscanf(id_file, "%d", &id) != EOF) {
            if (current_line == i - 1) {
                break;
            }
            current_line++;
        }
        fclose(id_file);

        if (current_line != i - 1) {
            fprintf(stderr, "Line number %d not found in ID file\n", i);
            return;
        }

        char bin_file_path[256];
        snprintf(bin_file_path, sizeof(bin_file_path), "..\\DataBase\\Application_Not_Checked\\application_%d.bin", id);
        FILE *bin_file = fopen(bin_file_path, "rb");
        if (bin_file == NULL) {
            fprintf(stderr, "Failed to open binary file '%s': %s\n", bin_file_path, strerror(errno));
            return;
        }

        Application loan_apply;
        fread(&loan_apply, sizeof(loan_apply), 1, bin_file);
        fclose(bin_file);

        char loan_path[256];
        snprintf(loan_path, sizeof(loan_path), "..\\DataBase\\LOANS\\loan_%d.bin", loan_apply.loan_id);
        FILE *loan_file = fopen(loan_path, "rb");
        if (loan_file == NULL) {
            fprintf(stderr, "Failed to open loan file '%s': %s\n", loan_path, strerror(errno));
            return;
        }

        loan_type loan;
        fread(&loan, sizeof(loan), 1, loan_file);
        fclose(loan_file);

        if (!((loan_apply.amount_requested >= loan.min_amount && loan_apply.amount_requested <= loan.max_amount) &&
              (loan_apply.loan_duration >= loan.min_duration && loan_apply.loan_duration <= loan.max_duration))) {
            char file_name[256];
            snprintf(file_name, sizeof(file_name), "..\\DataBase\\Applications\\user_%d.bin", loan_apply.user_id);
            update_application_status(file_name, loan_apply.loan_application_id, "Declined");
            send_application_declined_email(loan_apply.user_id);
            delete_id_from_file("..\\DataBase\\Id_Of_applications.txt", loan_apply.loan_application_id);

            snprintf(file_name, sizeof(file_name), "..\\DataBase\\Application_Not_Checked\\application_%d.bin", loan_apply.loan_application_id);
            if (remove(file_name) == 0) {
                printf("File '%s' deleted successfully.\n", file_name);
                goto sir_ltema;
            } else {
                fprintf(stderr, "Error deleting file '%s': %s\n", file_name, strerror(errno));
            }
        }
    }
}