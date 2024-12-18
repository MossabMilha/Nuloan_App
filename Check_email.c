// Created by PC on 22-Nov-24.

#include <gtk/gtk.h>
#include "Check_email.h"
#include <time.h>
#include "Send_Email.h"
#include "hash_password.h"
#include "Bank_information.h"

// Global user data
User new_user;
extern char global_first_name[50];
extern char global_last_name[50];
extern char global_cin[10];
extern char global_email[50];
extern char global_birthday[11];
extern char global_password[50];

int code_2FA;

// Function to generate a 6-digit 2FA code
int generate_2FA_code() {
    srand(time(NULL));
    return rand() % 900000 + 100000;
}

// Function to send a 2FA code via email
void send_2FA_Email(const gchar *email) {
    code_2FA = generate_2FA_code();
    char message[1024];
    snprintf(message, sizeof(message),
         "Dear User,\n\n"
         "We are sending you this email to provide your verification code: %d\n\n"
         "Please note that this code is valid for 5 minutes only. If you did not initiate this request, kindly disregard this message.\n\n"
         "Thank you for your attention.\n\n"
         "Best regards,\n"
         "The Credit Manager Team",
         code_2FA);
    send_email(email, "NULOAN Verification", message);
}

// Function to check the entered 2FA code
void Check(GtkWidget *button, gpointer user_data) {
    GtkWidget *new_window = GTK_WIDGET(user_data);
    GtkWidget *Code = g_object_get_data(G_OBJECT(new_window), "Code");
    const gchar *Code_Text = gtk_editable_get_text(GTK_EDITABLE(Code));

    if (atoi(Code_Text) == code_2FA) {
        // Retrieve the number of users from a file
        FILE *number_of_users = fopen("..\\DataBase\\Number_Of_Users.txt", "rt");
        int number_of_users_int;
        fscanf(number_of_users, "%d", &number_of_users_int);
        fclose(number_of_users);

        // Assign user information
        new_user.user_id = number_of_users_int + 1;
        strcpy(new_user.role, "User");
        strcpy(new_user.first_name, global_first_name);
        strcpy(new_user.last_name, global_last_name);
        strcpy(new_user.cin, global_cin);
        strcpy(new_user.email, global_email);
        strcpy(new_user.birthday, global_birthday);

        // Hash and store the password
        unsigned char hash[32];
        hash_password(global_password, hash);
        memcpy(new_user.password_hashed, hash, 32);

        // Assign timestamps
        new_user.created_at = time(NULL);
        new_user.updated_at = time(NULL);
        strcpy(new_user.status, "Active");

        Bank_Information_open(new_window);
    } else {
        g_print("Incorrect Code\n");
    }
}

// Function to handle the 2FA email sending and UI updates
void Email_2FA(GtkWidget *button, gpointer user_data) {
    GtkWidget *new_window = GTK_WIDGET(user_data);
    GtkCssProvider *button_provider = gtk_css_provider_new();
    gtk_css_provider_load_from_string(button_provider,
                                      "button#login_button { "
                                      "   background: #FFFFFF; "
                                      "   color: #000000; "
                                      "   border-radius: 25px; "
                                      "   border: none; "
                                      "} "
                                      "button#login_button:hover { "
                                      "   background: #E5E5E5; "
                                      "} ");

    GtkWidget *signup_fix = g_object_get_data(G_OBJECT(new_window), "signup_fix");
    GtkWidget *email = g_object_get_data(G_OBJECT(new_window), "Show_Email");
    const gchar *email_to = gtk_label_get_text(GTK_LABEL(email));
    GtkWidget *Button = g_object_get_data(G_OBJECT(new_window), "Sent Message");

    // Disconnect previous handler and replace label
    g_signal_handlers_disconnect_by_func(Button, G_CALLBACK(Email_2FA), new_window);
    send_2FA_Email(email_to);
    gtk_button_set_label(GTK_BUTTON(Button), "Submit");

    // Add 2FA entry field
    GtkWidget *Code_2FA = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(Code_2FA), "Enter the Code you Received");
    gtk_fixed_put(GTK_FIXED(signup_fix), Code_2FA, 50, 300);
    gtk_widget_set_size_request(Code_2FA, 250, 25);

    GtkStyleContext *Code_2FA_context = gtk_widget_get_style_context(Code_2FA);
    gtk_style_context_add_provider(Code_2FA_context, GTK_STYLE_PROVIDER(button_provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    g_object_set_data(G_OBJECT(new_window), "Code", Code_2FA);

    g_signal_connect(Button, "clicked", G_CALLBACK(Check), new_window);
}

// Function to create a new window for email verification
void email_checker(const gchar *first_name, const gchar *last_name, const gchar *cin, const gchar *email, const gchar *birthday, const gchar *password) {

    strcpy(global_first_name, first_name);
    strcpy(global_last_name, last_name);
    strcpy(global_cin, cin);
    strcpy(global_email, email);
    strcpy(global_birthday, birthday);
    strcpy(global_password, password);

    GtkWidget *new_window = gtk_window_new();
    gtk_window_set_title(GTK_WINDOW(new_window), "New Window");
    gtk_window_set_default_size(GTK_WINDOW(new_window), 350, 600);
    gtk_window_set_resizable(GTK_WINDOW(new_window), FALSE);

    GtkCssProvider *label_provider = gtk_css_provider_new();
    gtk_css_provider_load_from_string(label_provider,
                                      "label { "
                                      "  color: #FFFFFF; "
                                      "  font-size: 18px; "
                                      "  font-weight: bold; "
                                      "  padding: 10px; "
                                      "  text-align: center; "
                                      "} ");

    GtkCssProvider *button_provider = gtk_css_provider_new();
    gtk_css_provider_load_from_string(button_provider,
                                      "button { "
                                      "  background: #FFFFFF; "
                                      "  color: #000000; "
                                      "  border-radius: 25px; "
                                      "  border: none; "
                                      "} "
                                      "button:hover { "
                                      "  background: #E5E5E5; "
                                      "} ");

    GtkWidget *new_fix = gtk_fixed_new();
    gtk_window_set_child(GTK_WINDOW(new_window), new_fix);

    GtkWidget *title = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(title), "<span font='20' weight='bold'>Verify Your Email</span>");
    gtk_fixed_put(GTK_FIXED(new_fix), title, 10, 50);
    gtk_style_context_add_provider(gtk_widget_get_style_context(title), GTK_STYLE_PROVIDER(label_provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    GtkWidget *Show_Email = gtk_label_new(NULL);
    gchar *markup = g_markup_printf_escaped("<span font='20' weight='bold'>%s</span>", email);
    gtk_label_set_markup(GTK_LABEL(Show_Email), markup);
    g_free(markup);
    gtk_fixed_put(GTK_FIXED(new_fix), Show_Email, 5, 100);
    gtk_style_context_add_provider(gtk_widget_get_style_context(Show_Email), GTK_STYLE_PROVIDER(label_provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    GtkWidget *Sent_Message = gtk_button_new_with_label("Send Message");
    gtk_fixed_put(GTK_FIXED(new_fix), Sent_Message, 50, 380);
    gtk_widget_set_size_request(Sent_Message, 250, 20);
    gtk_style_context_add_provider(gtk_widget_get_style_context(Sent_Message), GTK_STYLE_PROVIDER(button_provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    g_object_set_data(G_OBJECT(new_window), "signup_fix", new_fix);
    g_object_set_data(G_OBJECT(new_window), "Show_Email", Show_Email);
    g_object_set_data(G_OBJECT(new_window), "Sent Message", Sent_Message);
    g_signal_connect(Sent_Message, "clicked", G_CALLBACK(Email_2FA), new_window);

    gtk_widget_show(new_window);
}