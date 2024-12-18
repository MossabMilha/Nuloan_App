#include "Approve_Reject_Application.h"
#include "Structures/User.h"
#include "Structures/Application.h"
#include "Structures/Loans_Types.h"
#include <gtk/gtk.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "Super_Admin.h"
#include "Send_Email.h"

User user;
Application loan_application;
loan_type loan;
int loan_application_id;
float chosen_interest_rate;

float suggest_interest_rate(int score, loan_type loan) {
    float interest_rate;
    if (score > 750) {
        interest_rate = loan.interest_rate[0];
    } else if (score > 650) {
        interest_rate = (loan.interest_rate[0] + loan.interest_rate[1]) / 2;
    } else {
        interest_rate = loan.interest_rate[1];
    }
    return interest_rate;
}

int calculate_credit_score(User user, Application loan_application, loan_type loan) {
    int score = 300;
    time_t current_time = time(NULL);

    if (loan_application.loan_duration >= loan.min_duration && loan_application.loan_duration <= loan.max_duration) {
        score += 30;
    } else {
        score -= 30;
    }

    double account_age_years = difftime(current_time, user.created_at) / (365.0 * 24.0 * 60.0 * 60.0);
    if (account_age_years > 5) {
        score += 100;
    } else if (account_age_years > 2) {
        score += 50;
    } else {
        score -= 20;
    }

    if (strcmp(user.status, "active") == 0) {
        score += 50;
    } else {
        score -= 50;
    }

    if (strcmp(loan_application.application_status, "Approved") == 0) {
        score += 30;
    } else if (strcmp(loan_application.application_status, "Declined") == 0) {
        score -= 50;
    } else if (strcmp(loan_application.application_status, "In Progress") == 0) {
        score += 10;
    }

    if (difftime(current_time, loan_application.repayment_start_date) < 0) {
        score += 10;
    } else {
        score -= 20;
    }

    if (score > 850) {
        score = 850;
    } else if (score < 300) {
        score = 300;
    }

    return score;
}

Application Read_application(int id) {
    char bin_file_path[256];
    snprintf(bin_file_path, sizeof(bin_file_path), "..\\DataBase\\Application_Not_Checked\\application_%d.bin", id);
    FILE *bin_file = fopen(bin_file_path, "rb");
    if (bin_file == NULL) {
        perror("Failed to open binary file");
        return (Application){0};
    }
    Application app;
    fread(&app, sizeof(app), 1, bin_file);
    fclose(bin_file);
    return app;
}

User Read_User_1(int id) {
    char bin_file_path[256];
    snprintf(bin_file_path, sizeof(bin_file_path), "..\\DataBase\\Users\\user_%d.bin", id);
    FILE *bin_file = fopen(bin_file_path, "rb");
    if (bin_file == NULL) {
        perror("Failed to open binary file");
        return (User){0};
    }
    User user;
    fread(&user, sizeof(user), 1, bin_file);
    fclose(bin_file);
    return user;
}

loan_type Read_Loan_1(int id) {
    char bin_file_path[256];
    snprintf(bin_file_path, sizeof(bin_file_path), "..\\DataBase\\LOANS\\loan_%d.bin", id);
    FILE *bin_file = fopen(bin_file_path, "rb");
    if (bin_file == NULL) {
        perror("Failed to open binary file");
        return (loan_type){0};
    }
    loan_type loan;
    fread(&loan, sizeof(loan), 1, bin_file);
    fclose(bin_file);
    return loan;
}
void send_loan_approval_email(int user_id, float loan_amount, float interest_rate, time_t repayment_start_date, float total_repayment) {
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

    // Format repayment start date
    char repayment_start_date_str[100];
    struct tm *tm_info = localtime(&repayment_start_date);
    strftime(repayment_start_date_str, sizeof(repayment_start_date_str), "%B %d, %Y", tm_info);

    // Create the email message
    char message[1024];
    snprintf(message, sizeof(message),
         "Dear %s,\n\n"
         "We are pleased to inform you that your loan application has been approved.\n\n"
         "Loan Details:\n"
         "Amount Requested: $%.2f\n"
         "Interest Rate: %.2f%%\n"
         "Repayment Start Date: %s\n"
         "Total Repayment: $%.2f\n\n"
         "We thank you for choosing NULOAN, and we look forward to helping you with your financial needs.\n\n"
         "Best regards,\n"
         "The NULOAN Team",
         user.first_name, loan_amount, interest_rate, repayment_start_date_str, total_repayment);

    send_email(user.email, "Loan Application Approved", message);  // Assuming you have a send_email function
}

void Approve_Application(GtkWidget *window) {
    Application updated_application = loan_application;
    strcpy(updated_application.application_status, "Approved");
    updated_application.interest_rate = chosen_interest_rate;

    struct tm *tm_info;
    time_t current_time = time(NULL);
    tm_info = localtime(&current_time);
    if (tm_info->tm_mday > 10) {
        tm_info->tm_mon += 2;
    } else {
        tm_info->tm_mon += 1;
    }
    tm_info->tm_mday = 1;
    updated_application.repayment_start_date = mktime(tm_info);
    updated_application.total_repayment = updated_application.amount_requested * (1 + chosen_interest_rate / 100);
    updated_application.updated_at = current_time;

    char user_bin_file_path[256];
    snprintf(user_bin_file_path, sizeof(user_bin_file_path), "..\\DataBase\\Applications\\user_%d.bin", updated_application.user_id);

    FILE *user_bin_file = fopen(user_bin_file_path, "rb+");
    if (user_bin_file == NULL) {
        perror("Failed to open binary file");
        return;
    }

    Application temp_application;
    while (fread(&temp_application, sizeof(Application), 1, user_bin_file) == 1) {
        if (temp_application.loan_application_id == updated_application.loan_application_id) {
            fseek(user_bin_file, -sizeof(Application), SEEK_CUR);
            fwrite(&updated_application, sizeof(Application), 1, user_bin_file);
            fclose(user_bin_file);
            printf("Loan application approved and updated successfully.\n");

            send_loan_approval_email(updated_application.user_id, updated_application.amount_requested, updated_application.interest_rate, updated_application.repayment_start_date, updated_application.total_repayment);
        }
    }

    char application_path[256];
    snprintf(application_path, sizeof(application_path), "..\\DataBase\\Application_Not_Checked\\application_%d.bin", updated_application.loan_application_id);
    printf("%s\n", application_path);
    if (remove(application_path) == 0) {
        printf("Application removed from the not checked folder\n");
    }

    // Now we will remove the ID from the Id_Of_applications.txt
    char id_path[256];
    snprintf(id_path, sizeof(id_path), "..\\DataBase\\Id_Of_applications.txt");

    // Open the Id_Of_applications.txt file for reading
    FILE *id_file = fopen(id_path, "r");
    if (id_file == NULL) {
        perror("Failed to open ID file");
        return;
    }

    // Temporary file to store the new contents without the deleted ID
    FILE *temp_file = fopen("temp.txt", "w");
    if (temp_file == NULL) {
        perror("Failed to open temporary file");
        fclose(id_file);
        return;
    }

    int id;
    while (fscanf(id_file, "%d", &id) == 1) {
        if (id != updated_application.loan_application_id) {
            fprintf(temp_file, "%d\n", id);  // Write ID to temp file if it doesn't match
        }
    }

    fclose(id_file);
    fclose(temp_file);

    // Replace the original file with the temporary file
    if (remove(id_path) != 0) {
        perror("Failed to remove the original ID file");
    } else {
        if (rename("temp.txt", id_path) != 0) {
            perror("Failed to rename temporary file");
        } else {
            printf("ID successfully deleted from the file.\n");
        }
    }

    // Close the user file and finish
    fclose(user_bin_file);
    Admin_Page(NULL, window);
    gtk_widget_set_visible(window, FALSE);
}
void send_loan_rejection_email(int user_id) {
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

    // Create the email message
    char message[1024];
    snprintf(message, sizeof(message),
         "Dear %s,\n\n"
         "We regret to inform you that your loan application has been rejected.\n\n"
         "Unfortunately, your application did not meet the required criteria for approval.\n\n"
         "If you believe this decision was made in error or if you have any questions, please contact our support team for further assistance.\n\n"
         "Best regards,\n"
         "The NULOAN Team",
         user.first_name);

    // Send the rejection email
    send_email(user.email, "Loan Application Rejected", message);  // Assuming you have a send_email function
}
void Reject_Application(GtkWidget *window) {
    Application updated_application = loan_application;
    strcpy(updated_application.application_status, "Rejected");
    updated_application.updated_at = time(NULL);

    char user_bin_file_path[256];
    snprintf(user_bin_file_path, sizeof(user_bin_file_path), "..\\DataBase\\Applications\\user_%d.bin", updated_application.user_id);

    FILE *user_bin_file = fopen(user_bin_file_path, "rb+");
    if (user_bin_file == NULL) {
        perror("Failed to open binary file");
        return;
    }

    Application temp_application;
    while (fread(&temp_application, sizeof(Application), 1, user_bin_file) == 1) {
        if (temp_application.loan_application_id == updated_application.loan_application_id) {
            fseek(user_bin_file, -sizeof(Application), SEEK_CUR);
            fwrite(&updated_application, sizeof(Application), 1, user_bin_file);
            fclose(user_bin_file);
            printf("Loan application rejected and updated successfully.\n");
            send_loan_rejection_email(updated_application.user_id);
        }
    }

    char application_path[256];
    snprintf(application_path, sizeof(application_path), "..\\DataBase\\Application_Not_Checked\\application_%d.bin", updated_application.loan_application_id);
    printf("%s\n", application_path);
    if (remove(application_path) == 0) {
        printf("Application removed from the not checked folder\n");
    }

    // Now we will remove the ID from the Id_Of_applications.txt
    char id_path[256];
    snprintf(id_path, sizeof(id_path), "..\\DataBase\\Id_Of_applications.txt");

    // Open the Id_Of_applications.txt file for reading
    FILE *id_file = fopen(id_path, "r");
    if (id_file == NULL) {
        perror("Failed to open ID file");
        return;
    }

    // Temporary file to store the new contents without the deleted ID
    FILE *temp_file = fopen("temp.txt", "w");
    if (temp_file == NULL) {
        perror("Failed to open temporary file");
        fclose(id_file);
        return;
    }

    int id;
    while (fscanf(id_file, "%d", &id) == 1) {
        if (id != updated_application.loan_application_id) {
            fprintf(temp_file, "%d\n", id);  // Write ID to temp file if it doesn't match
        }
    }

    fclose(id_file);
    fclose(temp_file);
    Admin_Page(NULL, window);
}


void final_step(GtkWidget *app, gpointer user_data) {
    GtkWidget *window = GTK_WIDGET(user_data);
    GtkWidget *fix = g_object_get_data(G_OBJECT(window), "fix");
    GtkWidget *generate_score_button = g_object_get_data(G_OBJECT(window), "generate_score_button");
    gtk_widget_set_visible(generate_score_button, FALSE);
    GtkWidget *Approve_button = g_object_get_data(G_OBJECT(window), "Approve_button");
    GtkWidget *Reject_button = g_object_get_data(G_OBJECT(window), "Reject_button");
    GtkWidget *exit = g_object_get_data(G_OBJECT(window), "exit");

    if (generate_score_button) {
        gtk_widget_set_visible(generate_score_button, FALSE);
    }
    if (Approve_button) {
        gtk_widget_set_visible(Approve_button, TRUE);
        gtk_widget_show(Approve_button);
        g_signal_connect(Approve_button, "clicked", G_CALLBACK(Approve_Application), window);
    }
    if (Reject_button) {
        gtk_widget_set_visible(Reject_button, TRUE);
        gtk_widget_show(Reject_button);
        g_signal_connect(Reject_button, "clicked", G_CALLBACK(Reject_Application), window);
    }
    if (exit) {
        gtk_widget_set_visible(exit, TRUE);
        gtk_widget_show(exit);
        g_signal_connect(exit, "clicked", G_CALLBACK(Admin_Page), window);
    }


    int score = calculate_credit_score(user, loan_application, loan);
    chosen_interest_rate = suggest_interest_rate(score, loan);

    // Combine the two texts into one string
    char *combined_text = g_strdup_printf(
        "<span font='10' weight='bold' foreground='red'>Score: %d  |  Interest Suggest: %.2f%%</span>",
        score, chosen_interest_rate);

    // Create a single label with the combined text
    GtkWidget *combined_label = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(combined_label), combined_text);

    // Free the combined text after using it
    g_free(combined_text);


    // Add the label to the fixed container
    gtk_fixed_put(GTK_FIXED(fix), combined_label, 100, 420);

    gtk_widget_queue_draw(window); // Ensure the window is redrawn
}

void Approve_Activate(GtkApplication *app, gpointer user_data) {
    gpointer *tab = (gpointer *)user_data; // Cast user_data back to gpointer array
    int loan_application_id = GPOINTER_TO_INT(tab[0]); // Retrieve loan ID
    GtkWidget *Apply_Loan_window = (GtkWidget *)tab[1]; // Retrieve parent window

    printf("Loan Application ID: %d\n", loan_application_id);

    loan_application = Read_application(loan_application_id);
    user = Read_User_1(loan_application.user_id);
    loan = Read_Loan_1(loan_application.loan_id);

    GtkWidget *window = gtk_window_new();
    gtk_window_set_title(GTK_WINDOW(window), "Loan Application Details");
    gtk_window_set_default_size(GTK_WINDOW(window), 477, 600);
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);

    // CSS Provider
    GtkCssProvider *css_provider = gtk_css_provider_new();
    gtk_css_provider_load_from_string(css_provider,
        "label.title { "
        "   font-size: 18px; "
        "   font-weight: bold; "
        "   color: #FFFFFF; "
        "   text-align: center; "
        "} "
        "label.static { "
        "   font-size: 20px; "
        "   font-weight: bold; "
        "   color: #FFFFFF; "
        "} "
        "label.dynamic { "
        "   font-size: 18px; "
        "   color: #FFFFFF; "
        "} "
        "button { "
        "   background-color: #4a90e2; "
        "   color: #FFFFFF; "
        "} "
        "frame { "
        "   border: 2px solid #FFFFFF; "
        "   padding: 5px; "
        "   margin: 5px; "
        "}");

    GtkStyleContext *window_context = gtk_widget_get_style_context(window);
    gtk_style_context_add_provider(window_context, GTK_STYLE_PROVIDER(css_provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    // Fixed layout for the window
    GtkWidget *fix = gtk_fixed_new();

    // Title
    GtkWidget *title_label = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(title_label), "<span font='18' weight='bold' color='#FFFFFF'>Loan Information</span>");
    gtk_fixed_put(GTK_FIXED(fix), title_label, 75, 5);
    gtk_widget_set_size_request(title_label, 320, -1);
    gtk_widget_set_halign(title_label, GTK_ALIGN_CENTER);


    // Frame
    GtkWidget *frame = gtk_frame_new(NULL);
    gtk_widget_set_size_request(frame, 320, 400);
    gtk_fixed_put(GTK_FIXED(fix), frame, 10, 50);
    GtkWidget *frame_fix = gtk_fixed_new();
    gtk_frame_set_child(GTK_FRAME(frame), frame_fix);

    // Helper macro to create rows
    #define ADD_ROW(static_text, dynamic_value, y_offset) \
        do { \
            GtkWidget *label_static = gtk_label_new(static_text); \
            gtk_style_context_add_provider(gtk_widget_get_style_context(label_static), \
                                           GTK_STYLE_PROVIDER(css_provider), GTK_STYLE_PROVIDER_PRIORITY_USER); \
            gtk_widget_add_css_class(label_static, "static"); \
            gtk_fixed_put(GTK_FIXED(frame_fix), label_static, 10, y_offset); \
            GtkWidget *label_dynamic = gtk_label_new(dynamic_value); \
            gtk_style_context_add_provider(gtk_widget_get_style_context(label_dynamic), \
                                           GTK_STYLE_PROVIDER(css_provider), GTK_STYLE_PROVIDER_PRIORITY_USER); \
            gtk_widget_add_css_class(label_dynamic, "dynamic"); \
            gtk_fixed_put(GTK_FIXED(frame_fix), label_dynamic, 195, y_offset); \
        } while(0)

    // Loan Details Rows
    char buffer[100];
    sprintf(buffer, "%d", loan_application.loan_application_id);
    ADD_ROW("Loan Id:", buffer, 10);

    ADD_ROW("Loan Type:", loan.loan_title, 40);
    ADD_ROW("Application Status:", loan_application.application_status, 70);
    ADD_ROW("Application Date:", loan_application.application_date, 100);

    sprintf(buffer, "%.2f", loan_application.interest_rate);
    ADD_ROW("Interest:", buffer, 130);

    ADD_ROW("Repayment Start Day:", ctime(&loan_application.repayment_start_date), 160);

    sprintf(buffer, "%d Months", loan_application.loan_duration);
    ADD_ROW("Loan Duration:", buffer, 190);

    ADD_ROW("Loan Description:", loan.loan_description, 220);

    // Buttons (approve, reject, generate score)
    GtkWidget *generate_score_button = gtk_button_new_with_label("Generate Score");
    gtk_fixed_put(GTK_FIXED(fix), generate_score_button, 10, 465);
    gtk_widget_set_size_request(generate_score_button, 455, 20);

    GtkWidget *approve_button = gtk_button_new_with_label("Approve");
    gtk_fixed_put(GTK_FIXED(fix), approve_button, 10, 465);
    gtk_widget_set_size_request(approve_button, 215, 20);
    gtk_widget_set_visible(approve_button, FALSE);

    GtkWidget *reject_button = gtk_button_new_with_label("Reject");
    gtk_fixed_put(GTK_FIXED(fix), reject_button, 250, 465);
    gtk_widget_set_size_request(reject_button, 215, 20);
    gtk_widget_set_visible(reject_button, FALSE);

    GtkWidget *exit = gtk_button_new_with_label("exit");
    gtk_fixed_put(GTK_FIXED(fix), exit, 10, 515);
    gtk_widget_set_size_request(exit, 455, 20);
    gtk_widget_set_visible(exit, FALSE);

    g_object_set_data(G_OBJECT(window), "fix", fix);
    g_object_set_data(G_OBJECT(window), "generate_score_button", generate_score_button);
    g_object_set_data(G_OBJECT(window), "Approve_button", approve_button);
    g_object_set_data(G_OBJECT(window), "Reject_button", reject_button);
    g_object_set_data(G_OBJECT(window), "exit", exit);

    g_signal_connect(generate_score_button, "clicked", G_CALLBACK(final_step), window);

    gtk_window_set_child(GTK_WINDOW(window), fix);
    gtk_window_present(GTK_WINDOW(window));
    gtk_widget_set_visible(Apply_Loan_window, FALSE);
}
