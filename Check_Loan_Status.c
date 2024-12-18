// Created by PC on 03-Dec-24.

#include "Check_Loan_Status.h"
#include "Active_Member.h"
#include "Structures/Application.h"
#include "Structures/Loans_Types.h"
#include "Check_Loan_Status_more.h"

// Utility function to open a binary file and handle errors

// Reads application information from the file
Application Read_Application_Information(int id, int struct_number) {
    char filename[100];
    sprintf(filename, "..\\DataBase\\Applications\\user_%d.bin", id);
    FILE *file = fopen(filename, "rb");

    Application user;
    fseek(file, (struct_number - 1) * sizeof(Application), SEEK_SET);
    fread(&user, sizeof(Application), 1, file);
    fclose(file);
    printf("loan id %d\n", user.loan_id);
    return user;
}

// Reads loan information from the file
loan_type Read_loan_Information(int id) {
    char filename[100];
    sprintf(filename, "..\\DataBase\\LOANS\\loan_%d.bin", id);
    FILE *file = fopen(filename, "rb");
    loan_type loan;
    fread(&loan, sizeof(loan_type), 1, file);
    fclose(file);
    return loan;
}

// Utility function to set markup text for a label
void SetMarkupText(GtkWidget *label, const char *format, ...) {
    char buffer[1000];
    va_list args;
    va_start(args, format);
    vsprintf(buffer, format, args);
    va_end(args);
    gtk_label_set_markup(GTK_LABEL(label), buffer);
}

// CSS Providers for modular styling
GtkCssProvider* Create_CSS_Provider(const char *css) {
    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_string(provider, css);
    return provider;
}

// Creates a frame with application details
void Create_User_Frame_1(GtkWidget *parent, const Application app, const loan_type loan, int y_offset, GtkWidget *main_window,
                         GtkCssProvider *frame_provider, GtkCssProvider *button_provider, GtkCssProvider *label_provider) {
    gpointer *data = g_malloc(3 * sizeof(gpointer));
    data[0] = GINT_TO_POINTER(app.user_id);
    data[1] = GINT_TO_POINTER(app.loan_application_id);
    data[2] = main_window;

    // Frame
    GtkWidget *frame = gtk_frame_new(NULL);
    gtk_widget_set_size_request(frame, 380, 90);  // Frame size
    GtkStyleContext *frame_context = gtk_widget_get_style_context(frame);
    gtk_style_context_add_provider(frame_context, GTK_STYLE_PROVIDER(frame_provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    GtkWidget *frame_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8); // Spacing inside the frame
    gtk_frame_set_child(GTK_FRAME(frame), frame_box);

    // Application ID
    GtkWidget *app_id_label = gtk_label_new(NULL);
    SetMarkupText(app_id_label, "<span font='10' weight='bold' color='#FFFFFF'>Application ID: %d</span>", app.loan_application_id);
    GtkStyleContext *label_context = gtk_widget_get_style_context(app_id_label);
    gtk_style_context_add_provider(label_context, GTK_STYLE_PROVIDER(label_provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    // Loan Type
    GtkWidget *loan_type_label = gtk_label_new(NULL);
    SetMarkupText(loan_type_label, "<span font='10' color='#FFFFFF'>Loan Type: %s</span>", loan.loan_title);

    // Application Status
    GtkWidget *status_label = gtk_label_new(NULL);
    SetMarkupText(status_label, "<span font='10' weight='bold' color='#FFFFFF'>Status:</span> <span>%s</span>", app.application_status);

    // More Information Button
    GtkWidget *more_button = gtk_button_new_with_label("More Information");
    gtk_widget_set_size_request(more_button, 80, 30); // Small width and height
    gtk_widget_set_margin_top(more_button, 2);
    gtk_widget_set_margin_bottom(more_button, 2);
    gtk_widget_set_margin_start(more_button, 2);
    gtk_widget_set_margin_end(more_button, 2);
    GtkStyleContext *button_context = gtk_widget_get_style_context(more_button);
    gtk_style_context_add_provider(button_context, GTK_STYLE_PROVIDER(button_provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    g_signal_connect(more_button, "clicked", G_CALLBACK(app_more_info), data);

    // Add widgets to the frame box
    gtk_box_append(GTK_BOX(frame_box), app_id_label);
    gtk_box_append(GTK_BOX(frame_box), loan_type_label);
    gtk_box_append(GTK_BOX(frame_box), status_label);
    gtk_box_append(GTK_BOX(frame_box), more_button);

    gtk_fixed_put(GTK_FIXED(parent), frame, 10, y_offset); // Adjusted position
}

// Builds the Loan Status UI
void Loan_Status_UI(GtkWidget *widget, gpointer user_data) {
    GtkWidget *main_window = GTK_WIDGET(user_data);
    int id = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(main_window), "id"));

    GtkWidget *loan_window = gtk_window_new();
    gtk_window_set_title(GTK_WINDOW(loan_window), "Loan Status");
    gtk_window_set_default_size(GTK_WINDOW(loan_window), 400, 660); // Slightly smaller window size
    gtk_window_set_resizable(GTK_WINDOW(loan_window), FALSE);
    g_object_set_data(G_OBJECT(loan_window), "id", GINT_TO_POINTER(id));

    GtkWidget *fixed_container = gtk_fixed_new();
    gtk_window_set_child(GTK_WINDOW(loan_window), fixed_container);

    // CSS Providers
    GtkCssProvider *frame_provider = Create_CSS_Provider(
        "frame { "
        "   border: 1px solid #A9A9A9; "
        "   border-radius: 10px; "
        "   background-color: rgba(10, 38, 71, 0.3); "
        "} ");

    GtkCssProvider *button_provider = Create_CSS_Provider(
    "button { "
    "   background: linear-gradient(to bottom, #E0E0E0, #C0C0C0); "
    "   color: #2C2C2C; "
    "   font-size: 15px; "      // Smaller font size
    "   border-radius: 5px; "   // Smaller corner radius
    "   padding: 2px 5px; "     // Minimal padding
    "   min-height: 20px; "     // Set a fixed height
    "   min-width: 60px; "      // Set a fixed width
    "} "
    "button:hover { background: #D3D3D3; }");

    GtkCssProvider *label_provider = Create_CSS_Provider(
        "label { "
        "   color: #FFFFFF; "
        "   font-size: 15px; "
        "} ");

    // Exit button
    GtkWidget *exit_button = gtk_button_new_with_label("Exit");
    gtk_widget_set_size_request(exit_button, 370, 35);
    gtk_fixed_put(GTK_FIXED(fixed_container), exit_button, 15, 575);
    g_signal_connect(exit_button, "clicked", G_CALLBACK(Active_Member_Page), loan_window);

    // Display application frames

    for (int i = 0; i < 4; i++) {
        Application app = Read_Application_Information(id, i + 1);
        printf("%d\n", app.loan_id);
        loan_type loan = Read_loan_Information(app.loan_id);
        Create_User_Frame_1(fixed_container, app, loan, 20 + i * 140, loan_window, frame_provider, button_provider, label_provider);
    }

    gtk_widget_set_visible(loan_window, TRUE);
    gtk_widget_set_visible(main_window, FALSE);
}
