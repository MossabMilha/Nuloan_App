// Created by PC on 14-Dec-24.

#include "Check_Loan_Status_more.h"
#include "Structures/Application.h"
#include "Structures/User.h"
#include "Structures/Loans_Types.h"
#include "Check_Loan_Status.h"

// Function to read application details from a binary file
Application Read_application_A(int user_id, int app_id) {
    char path[256];
    sprintf(path, "..\\DataBase\\Applications\\user_%d.bin", user_id);
    FILE *file = fopen(path, "rb");
    if (file == NULL) {
        perror("Failed to open binary file");
        return (Application){0};  // Return an empty Application structure
    }

    Application app;
    while (fread(&app, sizeof(Application), 1, file) == 1) {
        if (app.loan_application_id == app_id) {
            fclose(file);
            return app;  // Return the matching application
        }
    }
    fclose(file);
    return (Application){0};  // Return an empty Application structure if not found
}

// Function to read loan details from a binary file
loan_type Read_Loan_A(int id) {
    char path[256];
    sprintf(path, "..\\DataBase\\LOANS\\loan_%d.bin", id);
    FILE *file = fopen(path, "rb");
    if (file == NULL) {
        perror("Failed to open binary file");
        return (loan_type){0};  // Return an empty loan_type structure if file doesn't exist
    }

    loan_type loan;
    fread(&loan, sizeof(loan_type), 1, file);
    fclose(file);
    return loan;  // Return the loan details
}

// Function to display loan application details in a new window
void app_more_info(GtkApplication *app, gpointer user_data) {
    gpointer *data = (gpointer *)user_data;

    // Retrieve user_id and loan_application_id
    int user_id = GPOINTER_TO_INT(data[0]);
    int loan_application_id = GPOINTER_TO_INT(data[1]);
    GtkWidget *parent = (GtkWidget *)data[2];

    // Read application and loan details
    Application loan_application = Read_application_A(user_id, loan_application_id);
    loan_type loan = Read_Loan_A(loan_application.loan_id);

    // Create a new window
    GtkWidget *window = gtk_window_new();
    gtk_window_set_title(GTK_WINDOW(window), "Loan Application Details");
    gtk_window_set_default_size(GTK_WINDOW(window), 370, 600);
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);

    // CSS Provider
    GtkCssProvider *css_provider = gtk_css_provider_new();
    gtk_css_provider_load_from_string(css_provider,
        "label.title { "
        "   font-size: 18px; "
        "   font-weight: bold; "
        "   color: #FFFFFF; "
        "   text-align: center; "
        "}"
        "label.static { "
        "   font-size: 14px; "
        "   font-weight: bold; "
        "   color: #FFFFFF; "
        "}"
        "label.dynamic { "
        "   font-size: 14px; "
        "   color: #FFFFFF; "
        "}"
        "frame { "
        "   border: 2px solid #FFFFFF; "
        "   padding: 5px; "
        "   margin: 5px; "
        "}"
        "button { "
        "   background-color: #4a90e2; "
        "   color: #FFFFFF; "
        "}");

    // Apply CSS globally
    GtkStyleContext *window_context = gtk_widget_get_style_context(window);
    gtk_style_context_add_provider(window_context, GTK_STYLE_PROVIDER(css_provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    // Fixed layout for the window
    GtkWidget *fix = gtk_fixed_new();

    // Title
    GtkWidget *title_label = gtk_label_new("Loan Information");
    GtkStyleContext *title_context = gtk_widget_get_style_context(title_label);
    gtk_style_context_add_class(title_context, "title");
    gtk_style_context_add_provider(title_context, GTK_STYLE_PROVIDER(css_provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    gtk_fixed_put(GTK_FIXED(fix), title_label, 25, 10);
    gtk_widget_set_size_request(title_label, 350, -1);
    gtk_widget_set_halign(title_label, GTK_ALIGN_CENTER);

    // Frame
    GtkWidget *frame = gtk_frame_new(NULL);
    gtk_widget_set_size_request(frame, 320, 400);
    gtk_fixed_put(GTK_FIXED(fix), frame, 10, 50);

    GtkWidget *frame_fix = gtk_fixed_new();
    gtk_frame_set_child(GTK_FRAME(frame), frame_fix);

    // Helper macro to create rows and apply CSS
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
            gtk_fixed_put(GTK_FIXED(frame_fix), label_dynamic, 160, y_offset); \
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

    // Back Button
    GtkWidget *back_button = gtk_button_new_with_label("Back");
    gtk_fixed_put(GTK_FIXED(fix), back_button, 25, 470);
    gtk_widget_set_size_request(back_button, 320, 20);
    g_object_set_data(G_OBJECT(window), "id", GINT_TO_POINTER(user_id));
    g_signal_connect(back_button, "clicked", G_CALLBACK(Loan_Status_UI), window);

    // Finalize window setup
    gtk_window_set_child(GTK_WINDOW(window), fix);
    gtk_window_present(GTK_WINDOW(window));
    gtk_widget_set_visible(parent, FALSE);

    g_free(data);
}



