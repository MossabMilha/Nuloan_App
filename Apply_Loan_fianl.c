#include "Apply_Loan_More_Information.h"
#include "Apply_Loan_fianl.h"
#include "Structures/Loans_Types.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
#include <time.h>
#include "Structures/Application.h"
#include "Active_Member.h"
#include "Send_Email.h"
#include "Structures/User.h"

// Array to store loan details
loan_type loans[6];

// Function to generate a unique ID
int generate_unique_id() {
    int temp_id, existing_id;
    int is_unique;
    FILE *id_file;

    srand((unsigned int)time(NULL));

    do {
        is_unique = 1;
        temp_id = rand() % 900000 + 100000;

        id_file = fopen("..\\DataBase\\Id_Of_applications.txt", "r");
        if (id_file == NULL) {
            perror("Failed to open ID file for reading");
            return -1;
        }

        while (fscanf(id_file, "%d", &existing_id) != EOF) {
            if (temp_id == existing_id) {
                is_unique = 0;
                break;
            }
        }
        fclose(id_file);
    } while (!is_unique);

    id_file = fopen("..\\DataBase\\Id_Of_applications.txt", "a");
    if (id_file == NULL) {
        perror("Failed to open ID file for appending");
        return -1;
    }

    fprintf(id_file, "%d\n", temp_id);
    fclose(id_file);

    return temp_id;
}
void send_loan_application_email(const gchar *email, const gchar *first_name, const gchar *last_name,
                                 int loan_application_id, float amount_requested, int loan_duration) {
    char message[1024];
    snprintf(message, sizeof(message),
        "Dear %s %s,\n\n"
        "Thank you for applying for a loan with NULOAN.\n\n"
        "Here are the details of your loan application:\n"
        "- Loan Application ID: %d\n"
        "- Amount Requested: %.2f\n"
        "- Loan Duration: %d months\n\n"
        "Your application status is currently 'In Progress.' We will notify you via email regarding any updates about your loan application.\n\n"
        "If you have any questions, feel free to contact us at support@nuloan.com.\n\n"
        "Thank you for choosing NULOAN.\n\n"
        "Best regards,\n"
        "The NULOAN Team",
        first_name, last_name, loan_application_id, amount_requested, loan_duration);

    send_email(email, "Your NULOAN Application Details", message);
}
// Save loan application information
void save_information(GtkWidget *widget, gpointer user_data) {
    GtkWidget *loan_window = GTK_WIDGET(user_data);

    GtkWidget *amount_entry = g_object_get_data(G_OBJECT(loan_window), "amount_entry");
    GtkWidget *duration_entry = g_object_get_data(G_OBJECT(loan_window), "duration_entry");
    GtkWidget *income_entry = g_object_get_data(G_OBJECT(loan_window), "income_entry");
    gint user_id = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(loan_window), "user_id"));
    gint loan_id = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(loan_window), "loan_id"));
    printf("user id : %d\n", user_id);

    if (!GTK_IS_ENTRY(amount_entry) || !GTK_IS_ENTRY(duration_entry) || !GTK_IS_ENTRY(income_entry)) {
        g_critical("Invalid GtkEntry widgets.");
        return;
    }

    GtkEntryBuffer *amount_entry_buffer = gtk_entry_get_buffer(GTK_ENTRY(amount_entry));
    const gchar *amount_text = gtk_entry_buffer_get_text(amount_entry_buffer);

    GtkEntryBuffer *duration_entry_buffer = gtk_entry_get_buffer(GTK_ENTRY(duration_entry));
    const gchar *duration_text = gtk_entry_buffer_get_text(duration_entry_buffer);

    GtkEntryBuffer *income_entry_buffer = gtk_entry_get_buffer(GTK_ENTRY(income_entry));
    const gchar *income_text = gtk_entry_buffer_get_text(income_entry_buffer);


    float amount_needed = strtof(amount_text, NULL);
    int duration_needed = atoi(duration_text);
    float income = strtof(income_text, NULL);

    Application app;
    app.loan_application_id = generate_unique_id();
    app.user_id = user_id;
    app.loan_id = loan_id + 1;
    printf("loan id :%d  \n", app.loan_id);
    app.amount_requested = amount_needed;
    strcpy(app.application_status, "In Progress");
    time_t now = time(NULL);
    strftime(app.application_date, sizeof(app.application_date), "%d-%m-%Y", localtime(&now));
    app.loan_duration = duration_needed;
    app.income = income;


    char filename_admin[256];
    snprintf(filename_admin, sizeof(filename_admin), "..\\DataBase\\Application_Not_Checked\\application_%d.bin", app.loan_application_id);
    FILE *file_admin = fopen(filename_admin, "wb");
    if (file_admin == NULL) {
        perror("Unable to open file");
        return;
    }
    fwrite(&app, sizeof(app), 1, file_admin);
    fclose(file_admin);

    char filename_user[256];
    snprintf(filename_user, sizeof(filename_user), "..\\DataBase\\Applications\\user_%d.bin", user_id);
    FILE *file_user = fopen(filename_user, "ab");
    if (file_user == NULL) {
        perror("Unable to open file");
        return;
    }
    fwrite(&app, sizeof(app), 1, file_user);
    fclose(file_user);
    char path[256];
    snprintf(path, sizeof(path), "..\\DataBase\\Users\\user_%d.bin", app.user_id);
    User user;
    FILE *file = fopen(path, "rb");
    if (file == NULL) {
        perror("Unable to open file");
        return;
    }
    fread(&user, sizeof(User), 1, file);
    fclose(file);
    send_loan_application_email(user.email, user.first_name, user.last_name, app.loan_application_id, amount_needed, duration_needed);
    g_object_set_data(G_OBJECT(loan_window), "id", GINT_TO_POINTER(app.user_id));

    GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(loan_window), GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "Your loan application was successful!\nWe will notify you about any updates.");
    g_signal_connect(dialog, "response", G_CALLBACK(gtk_window_close), NULL);
    gtk_widget_show(dialog);

    Active_Member_Page(widget, loan_window);
}

void Final_Apply(GtkWidget *widget, gpointer user_data) {
    GtkWidget *main_window = GTK_WIDGET(user_data);
    gint user_id = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(main_window), "user_id"));
    gint loan_id = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(main_window), "loan_id"));


    GtkWidget *loan_window = gtk_window_new();
    gtk_window_set_title(GTK_WINDOW(loan_window), "Apply For Loan");
    gtk_window_set_default_size(GTK_WINDOW(loan_window), 350, 600);
    gtk_window_set_resizable(GTK_WINDOW(loan_window), FALSE);

    gpointer *tab = g_malloc(sizeof(gpointer) * 3); // Allocate space for 3 items
    tab[0] = GINT_TO_POINTER(1);                  // Store id as a pointer
    tab[1] = GINT_TO_POINTER(1);               // Store index as a pointer
    tab[2] = loan_window;
    g_object_set_data(G_OBJECT(loan_window), "user_id", GINT_TO_POINTER(user_id));
    g_object_set_data(G_OBJECT(loan_window), "loan_id", GINT_TO_POINTER(loan_id));

    GtkWidget *loan_fixed = gtk_fixed_new();
    gtk_window_set_child(GTK_WINDOW(loan_window), loan_fixed);

    // Amount Requested Section
    GtkWidget *amount_label = gtk_label_new("");
    gtk_label_set_markup(GTK_LABEL(amount_label), "<span font='12' weight='bold' foreground='#ffffff'>Amount Requested:</span>");
    gtk_label_set_xalign(GTK_LABEL(amount_label), 0.1);
    gtk_fixed_put(GTK_FIXED(loan_fixed), amount_label, 25, 75);

    GtkWidget *amount_entry = gtk_entry_new();
    gtk_widget_set_size_request(amount_entry, 300, 30);
    gtk_fixed_put(GTK_FIXED(loan_fixed), amount_entry, 25, 100);

    // Loan Duration Section
    GtkWidget *duration_label = gtk_label_new("");
    gtk_label_set_markup(GTK_LABEL(duration_label), "<span font='12' weight='bold' foreground='#ffffff'>Loan Duration (months):</span>");
    gtk_label_set_xalign(GTK_LABEL(duration_label), 0.1);
    gtk_fixed_put(GTK_FIXED(loan_fixed), duration_label, 25, 150);

    GtkWidget *duration_entry = gtk_entry_new();
    gtk_widget_set_size_request(duration_entry, 300, 30);
    gtk_fixed_put(GTK_FIXED(loan_fixed), duration_entry, 25, 175);

    // Monthly Income Section
    GtkWidget *income_label = gtk_label_new("");
    gtk_label_set_markup(GTK_LABEL(income_label), "<span font='12' weight='bold' foreground='#ffffff'>Monthly Income:</span>");
    gtk_label_set_xalign(GTK_LABEL(income_label), 0.1);
    gtk_fixed_put(GTK_FIXED(loan_fixed), income_label, 25, 225);

    GtkWidget *income_entry = gtk_entry_new();
    gtk_widget_set_size_request(income_entry, 300, 30);
    gtk_fixed_put(GTK_FIXED(loan_fixed), income_entry, 25, 250);

    // Apply Button
    GtkWidget *apply_button = gtk_button_new_with_label("Apply");
    gtk_widget_set_size_request(apply_button, 300, 30);
    gtk_fixed_put(GTK_FIXED(loan_fixed), apply_button, 25, 300);
    g_object_set_data(G_OBJECT(loan_window), "amount_entry", amount_entry);
    g_object_set_data(G_OBJECT(loan_window), "duration_entry", duration_entry);
    g_object_set_data(G_OBJECT(loan_window), "income_entry", income_entry);
    g_signal_connect(apply_button, "clicked", G_CALLBACK(save_information), loan_window);

    // Go Back Button
    GtkWidget *back_button = gtk_button_new_with_label("Go Back");
    gtk_widget_set_size_request(back_button, 300, 30);
    gtk_fixed_put(GTK_FIXED(loan_fixed), back_button, 25, 350);
    g_signal_connect(back_button, "clicked", G_CALLBACK(Apply_Loan_Page_2_UI), tab);

    gtk_widget_set_visible(loan_window, TRUE);
    gtk_widget_set_visible(main_window, FALSE);
    gtk_widget_set_visible(widget, FALSE);
}
