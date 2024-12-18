#include <gtk/gtk.h>

#include "Check_Signup_Information.h"
#include "Check_User_Information.h"
#include "Check_email.h"

void check_SignUp(GtkWidget *widget, gpointer user_data) {
    // Retrieve the signup window from user_data
    GtkWidget *signup_window = GTK_WIDGET(user_data);
    int is_fields_empty = 0;
    int is_fields_invalid = 0;

    // Retrieve the first_name information
    GtkWidget *First_Name = g_object_get_data(G_OBJECT(signup_window), "First Name");
    GtkEntryBuffer *first_name_buffer = gtk_entry_get_buffer(GTK_ENTRY(First_Name));
    const gchar *first_name_text = gtk_entry_buffer_get_text(first_name_buffer);

    GtkWidget *First_Name_Button = g_object_get_data(G_OBJECT(signup_window), "First Name Image");

    if (strlen(first_name_text) == 0) {
        is_fields_empty = 1;
        gtk_widget_set_visible(First_Name_Button, TRUE);
        gtk_widget_set_tooltip_text(First_Name_Button, "First Name is required");
    } else {
        gtk_widget_set_visible(First_Name_Button, FALSE);
        check_first_name(first_name_text, First_Name_Button);
        is_fields_invalid = 0;
        if (gtk_widget_get_visible(First_Name_Button)) {
            gtk_widget_set_tooltip_text(First_Name_Button, "Enter A Valid First Name");
            is_fields_invalid = 1;
        }
    }

    // Retrieve the Last_name information
    GtkWidget *Last_Name = g_object_get_data(G_OBJECT(signup_window), "Last Name");
    GtkEntryBuffer *Last_Name_buffer = gtk_entry_get_buffer(GTK_ENTRY(Last_Name));
    const gchar *Last_Name_text = gtk_entry_buffer_get_text(Last_Name_buffer);

    GtkWidget *Last_Name_Button = g_object_get_data(G_OBJECT(signup_window), "Last Name Image");

    if (strlen(Last_Name_text) == 0) {
        is_fields_empty = 1;
        gtk_widget_set_visible(Last_Name_Button, TRUE);
        gtk_widget_set_tooltip_text(Last_Name_Button, "Last Name is required");
    } else {
        gtk_widget_set_visible(Last_Name_Button, FALSE);
        check_last_name(Last_Name_text, Last_Name_Button);
        is_fields_invalid = 0;
        if (gtk_widget_get_visible(Last_Name_Button)) {
            gtk_widget_set_tooltip_text(Last_Name_Button, "Enter A Valid Last Name");
            is_fields_invalid = 1;
        }
    }

    // Retrieve the CIN information
    GtkWidget *CIN = g_object_get_data(G_OBJECT(signup_window), "CIN");
    GtkEntryBuffer *CIN_buffer = gtk_entry_get_buffer(GTK_ENTRY(CIN));
    const gchar *CIN_text = gtk_entry_buffer_get_text(CIN_buffer);

    GtkWidget *CIN_Button = g_object_get_data(G_OBJECT(signup_window), "CIN Image");

    if (strlen(CIN_text) == 0) {
        is_fields_empty = 1;
        gtk_widget_set_visible(CIN_Button, TRUE);
        gtk_widget_set_tooltip_text(CIN_Button, "CIN is required");
    } else {
        gtk_widget_set_visible(CIN_Button, FALSE);
        check_CIN(CIN_text, CIN_Button);
        is_fields_invalid = 0;
        if (gtk_widget_get_visible(CIN_Button)) {
            is_fields_invalid = 1;
        }
    }

    // Retrieve the Email information
    GtkWidget *Email = g_object_get_data(G_OBJECT(signup_window), "Email");
    GtkEntryBuffer *Email_buffer = gtk_entry_get_buffer(GTK_ENTRY(Email));
    const gchar *Email_text = gtk_entry_buffer_get_text(Email_buffer);

    GtkWidget *Email_Button = g_object_get_data(G_OBJECT(signup_window), "Email Image");

    if (strlen(Email_text) == 0) {
        is_fields_empty = 1;
        gtk_widget_set_visible(Email_Button, TRUE);
        gtk_widget_set_tooltip_text(Email_Button, "Email is required");
    } else {
        gtk_widget_set_visible(Email_Button, FALSE);
        check_email(Email_text, Email_Button);
        is_fields_invalid = 0;
        if (gtk_widget_get_visible(Email_Button)) {
            is_fields_invalid = 1;
        }
    }

    // Retrieve the Birthday information
    GtkWidget *Birthday = g_object_get_data(G_OBJECT(signup_window), "Birthday DD//MM//YYY");
    GtkEntryBuffer *Birthday_buffer = gtk_entry_get_buffer(GTK_ENTRY(Birthday));
    const gchar *Birthday_text = gtk_entry_buffer_get_text(Birthday_buffer);

    GtkWidget *Birthday_Button = g_object_get_data(G_OBJECT(signup_window), "Birthday Image");

    if (strlen(Birthday_text) == 0) {
        is_fields_empty = 1;
        gtk_widget_set_visible(Birthday_Button, TRUE);
        gtk_widget_set_tooltip_text(Birthday_Button, "Birthday is required");
    } else {
        gtk_widget_set_visible(Birthday_Button, FALSE);
        check_birthday(Birthday_text, Birthday_Button);
        is_fields_invalid = 0;
        if (gtk_widget_get_visible(Birthday_Button)) {
            gtk_widget_set_tooltip_text(Email_Button, "Enter A Valid Birthday DD//MM//YYY");
            is_fields_invalid = 1;
        }
    }

    // Retrieve the Password information
    GtkWidget *Password = g_object_get_data(G_OBJECT(signup_window), "Password");
    GtkEntryBuffer *Password_buffer = gtk_entry_get_buffer(GTK_ENTRY(Password));

    GtkWidget *Confirm_Password = g_object_get_data(G_OBJECT(signup_window), "Confirm Password");
    GtkEntryBuffer *Confirm_Password_buffer = gtk_entry_get_buffer(GTK_ENTRY(Confirm_Password));

    const gchar *Password_text = gtk_entry_buffer_get_text(Password_buffer);
    const gchar *Confirm_Password_text = gtk_entry_buffer_get_text(Confirm_Password_buffer);

    GtkWidget *Password_Button = g_object_get_data(G_OBJECT(signup_window), "Password Image");

    if (strlen(Password_text) == 0 || strlen(Confirm_Password_text) == 0) {
        is_fields_empty = 1;
        gtk_widget_set_visible(Password_Button, TRUE);
        gtk_widget_set_tooltip_text(Password_Button, "The Password and The Confirmation Password is required");
    } else {
        gtk_widget_set_visible(Password_Button, FALSE);
        char *message = check_password(Password_text, Confirm_Password_text, first_name_text, Last_Name_text, Password_Button);
        is_fields_invalid = 0;
        if (gtk_widget_get_visible(Password_Button)) {
            gtk_widget_set_tooltip_text(Password_Button, message);
            is_fields_invalid = 1;
        }
    }

    if (is_fields_empty) {
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(signup_window), GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "%s", "ALL Fields Required");
        g_signal_connect(dialog, "response", G_CALLBACK(gtk_window_close), NULL);
        gtk_widget_show(dialog);
    }else if (is_fields_invalid) {

    }
    else {
        email_checker(first_name_text, Last_Name_text, CIN_text, Email_text, Birthday_text, Password_text);
        gtk_window_close(GTK_WINDOW(signup_window));
    }
}
