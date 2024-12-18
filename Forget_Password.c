    #include "Forget_Password.h"
    #include "Send_Email.h"
    #include <time.h>
    #include <stdlib.h>
    #include "Structures/User.h"
    #include "Check_User_Information.h"
    #include "Structures/User.h"
#include "hash_password.h"
#include "First_Page.h"
    int global_user_id;
    int global_code_2FA;
    int search_for_email(const gchar *email) {
        FILE *file_1 = fopen("..\\DataBase\\Number_Of_Users.txt", "r");
        if (file_1 == NULL) {
            return 0; // Unable to open file
        }

        int number_of_users;
        if (fscanf(file_1, "%d", &number_of_users) != 1) {
            fclose(file_1);
            return 0; // Failed to read user count
        }
        fclose(file_1);

        for (int i = 1; i <= number_of_users; i++) {
            char path[100];
            snprintf(path, sizeof(path), "..\\DataBase\\Users\\user_%d.bin", i);

            FILE *file = fopen(path, "rb");
            if (file == NULL) {
                continue; // Skip missing user files
            }

            User user;
            if (fread(&user, sizeof(User), 1, file) == 1) {
                if (strcmp(user.email, email) == 0) {
                    global_user_id = user.user_id;
                    fclose(file);
                    return 1; // Email found
                }
            }
            fclose(file);
        }
        return 0; // Email not found
    }
    void change_password_step2(GtkWidget *app,gpointer data) {

        GtkWidget *main_window = GTK_WIDGET(data);
        GtkWidget *forget_password_fix = g_object_get_data(G_OBJECT(data), "Fix");
        char path[255];
        snprintf(path, sizeof(path), "..\\DataBase\\Users\\user_%d.bin", global_user_id);
        FILE *file = fopen(path, "rb+");
        if (file == NULL) {
            return; // Unable to open file
        }
        User user;
        if (fread(&user, sizeof(User), 1, file) != 1) {
            fclose(file);
            return; // Unable to read user
        }



        GtkWidget *password_entry = g_object_get_data(G_OBJECT(data), "password_entry");
        GtkEntryBuffer *Password_buffer = gtk_entry_get_buffer(GTK_ENTRY(password_entry));
        const gchar *Password_text = gtk_entry_buffer_get_text(Password_buffer);

        GtkWidget *confirmation_password_entry = g_object_get_data(G_OBJECT(data), "confirmation_password_entry");
        GtkEntryBuffer *Confirmation_Password_buffer = gtk_entry_get_buffer(GTK_ENTRY(confirmation_password_entry));
        const gchar *Confirmation_Password_text = gtk_entry_buffer_get_text(Confirmation_Password_buffer);

        if (strlen(Password_text) == 0 || strlen(Confirmation_Password_text) == 0) {
            GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(main_window), GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "%s", "ALL Fields Required");
            g_signal_connect(dialog, "response", G_CALLBACK(gtk_window_close), NULL);
            gtk_widget_show(dialog);
        } else {
            char *message = check_password(Password_text, Confirmation_Password_text, user.first_name, user.last_name, NULL);
            if (strlen(message) == 0) {
                fseek(file, 0, SEEK_SET);
                unsigned char hashed_password[50];
                hash_password(Password_text, hashed_password);
                memcpy(user.password_hashed, hashed_password, sizeof(user.password_hashed));
                fwrite(&user, sizeof(User), 1, file);
                fclose(file);
                GtkWidget *success_dialog = gtk_message_dialog_new(GTK_WINDOW(main_window), GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "Password successfully changed!");
                g_signal_connect(success_dialog, "response", G_CALLBACK(gtk_window_close), NULL);
                gtk_widget_show(success_dialog);

                First_Page_UI(NULL, main_window);

            }else {
                GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(main_window), GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "%s", message);
                g_signal_connect(dialog, "response", G_CALLBACK(gtk_window_close), NULL);
                gtk_widget_show(dialog);
            }
        }
    }

    void change_password_step1(GtkWidget *app,gpointer data) {
        GtkWidget *main_window = GTK_WIDGET(data);
        GtkWidget *forget_password_fix = g_object_get_data(G_OBJECT(data), "Fix");

        GtkWidget *password_entry = gtk_entry_new();
        gtk_entry_set_placeholder_text(GTK_ENTRY(password_entry), "Enter your new password");
        gtk_fixed_put(GTK_FIXED(forget_password_fix), password_entry, 50, 200);
        gtk_widget_set_size_request(password_entry, 250, 20);
        gtk_entry_set_invisible_char(GTK_ENTRY(password_entry), '*');
        gtk_entry_set_visibility(GTK_ENTRY(password_entry), FALSE);  // Hide the text
        gtk_widget_show(password_entry);

        GtkWidget *confirmation_password_entry = gtk_entry_new();
        gtk_entry_set_placeholder_text(GTK_ENTRY(confirmation_password_entry), "Enter your new password");
        gtk_fixed_put(GTK_FIXED(forget_password_fix), confirmation_password_entry, 50, 240);
        gtk_widget_set_size_request(confirmation_password_entry, 250, 20);
        gtk_entry_set_invisible_char(GTK_ENTRY(confirmation_password_entry), '*');
        gtk_entry_set_visibility(GTK_ENTRY(confirmation_password_entry), FALSE);  // Hide the text
        gtk_widget_show(confirmation_password_entry);
        

        GtkWidget *change_password_button = gtk_button_new_with_label("Change Password");
        gtk_fixed_put(GTK_FIXED(forget_password_fix), change_password_button, 50, 280);


        g_object_set_data(G_OBJECT(main_window), "password_entry", password_entry);
        g_object_set_data(G_OBJECT(main_window), "confirmation_password_entry", confirmation_password_entry);

        g_signal_connect(change_password_button, "clicked", G_CALLBACK(change_password_step2), main_window);






        gtk_widget_show(main_window);

    }
void check_code(GtkWidget *widget, gpointer data) {
        printf("Check Code function called\n");

        GtkWidget *main_window = GTK_WIDGET(data);
        GtkWidget *code_entry = g_object_get_data(G_OBJECT(data), "code_entry");
        GtkEntryBuffer *code_buffer = gtk_entry_get_buffer(GTK_ENTRY(code_entry));
        const gchar *code_text = gtk_entry_buffer_get_text(code_buffer);

        if (strcmp(code_text, g_strdup_printf("%d", global_code_2FA)) == 0) {
            printf("Codes match! Proceeding to change password...\n");
            change_password_step1(main_window, data);
        } else {
            printf("Codes do not match.\n");
        }
    }
    void Send_Verification_Email(GtkWidget *widget, gpointer data) {
    GtkWidget *main_window = GTK_WIDGET(data);
    GtkWidget *forget_password_fix = g_object_get_data(G_OBJECT(data), "Fix");
    GtkWidget *email_entry = g_object_get_data(G_OBJECT(data), "Entry");
    GtkWidget *email_button = g_object_get_data(G_OBJECT(data), "Send Email");

    GtkEntryBuffer *email_buffer = gtk_entry_get_buffer(GTK_ENTRY(email_entry));
    const gchar *email_text = gtk_entry_buffer_get_text(email_buffer);

    gtk_widget_set_visible(email_entry, FALSE);

    GtkWidget *code_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(code_entry), "Enter Verification Code");
    gtk_fixed_put(GTK_FIXED(forget_password_fix), code_entry, 50, 200);
    gtk_widget_set_size_request(code_entry, 250, 20);
    gtk_widget_show(code_entry);

    gtk_widget_set_visible(email_button, FALSE);
    GtkWidget *code_button = gtk_button_new_with_label("Check Code");
    gtk_fixed_put(GTK_FIXED(forget_password_fix), code_button, 50, 240);
    gtk_widget_set_size_request(code_button, 250, 20);
    g_signal_connect(code_button, "clicked", G_CALLBACK(change_password_step1), main_window);

    g_object_set_data(G_OBJECT(main_window), "code_entry", code_entry);
    g_object_set_data(G_OBJECT(main_window), "code_button", code_button);

    if (search_for_email(email_text)) {
        printf("Email found. Sending verification email...\n");

        srand((unsigned int)time(NULL));
        global_code_2FA = rand() % 900000 + 100000;

        char subject[1024];
        snprintf(subject, sizeof(subject), "Password Reset Code for Your nuloan Account");

        char message[1024];
        snprintf(message, sizeof(message),
                 "Dear User,\n\n"
                 "We received a request to reset your password for your nuloan account. If you did not make this request, you can safely ignore this email.\n\n"
                 "To reset your password, please use the following code:\n"
                 "%d\n\n"
                 "This code is valid for 5 minutes. Please do not share it with anyone for your account’s security.\n\n"
                 "Thank you,\n"
                 "The nuloan Team",
                 global_code_2FA);

        send_email(email_text, subject, message);
        printf("Verification email sent with code: %d\n", global_code_2FA);

        g_object_set_data(G_OBJECT(main_window), "code_2FA", GINT_TO_POINTER(global_code_2FA));
    } else {
        printf("Email not found\n");
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(main_window), GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "Email not found");
        g_signal_connect(dialog, "response", G_CALLBACK(gtk_window_close), NULL);
        gtk_widget_show(dialog);
    }
}


    void Forget_Password_UI(GtkWidget *app, gpointer data) {
        GtkWidget *main_window = GTK_WIDGET(data);

        GtkWidget *forget_password_window = gtk_window_new();
        gtk_window_set_title(GTK_WINDOW(forget_password_window), "Forget Password");
        gtk_window_set_default_size(GTK_WINDOW(forget_password_window), 350, 600);
        gtk_window_set_resizable(GTK_WINDOW(forget_password_window), FALSE);

        GtkWidget *forget_password_fix = gtk_fixed_new();
        gtk_window_set_child(GTK_WINDOW(forget_password_window), forget_password_fix);

        GtkWidget *email_entry = gtk_entry_new();
        gtk_entry_set_placeholder_text(GTK_ENTRY(email_entry), "Enter your email");
        gtk_fixed_put(GTK_FIXED(forget_password_fix), email_entry, 50, 200);
        gtk_widget_set_size_request(email_entry, 250, 20);

        GtkWidget *email_button = gtk_button_new_with_label("Send Email");
        gtk_fixed_put(GTK_FIXED(forget_password_fix), email_button, 50, 240);
        gtk_widget_set_size_request(email_button, 250, 20);
        g_signal_connect(email_button, "clicked", G_CALLBACK(Send_Verification_Email), forget_password_window);

        g_object_set_data(G_OBJECT(forget_password_window), "Fix", forget_password_fix);
        g_object_set_data(G_OBJECT(forget_password_window), "Entry", email_entry);
        g_object_set_data(G_OBJECT(forget_password_window), "Send Email", email_button);


        gtk_widget_set_visible(forget_password_window, TRUE);
        gtk_widget_set_visible(main_window, FALSE);
    }