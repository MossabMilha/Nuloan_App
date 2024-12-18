//
// Created by PC on 01-Dec-24.
//

#include "Login_Process_Handle_Window.h"
#include "hash_password.h"
#include "Structures/User.h"
#include "Super_Admin.h"
#include "Active_Member.h"
#include <string.h>


void check_login(GtkWidget *widget, gpointer user_data) {

    GtkWidget *main_window = GTK_WIDGET(user_data);
    GtkWidget *id_entry = g_object_get_data(G_OBJECT(main_window), "id_entry");
    GtkEntryBuffer *id_buffer = gtk_entry_get_buffer(GTK_ENTRY(id_entry));
    const gchar *id = gtk_entry_buffer_get_text(id_buffer);

    GtkWidget *password_entry = g_object_get_data(G_OBJECT(main_window), "password_entry");
    GtkEntryBuffer *password_buffer = gtk_entry_get_buffer(GTK_ENTRY(password_entry));
    const gchar *password = gtk_entry_buffer_get_text(password_buffer);
    unsigned char hashed_password[32];
    if (strlen(id)==0||strlen(password)==0) {
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(main_window), GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "%s", "The ID or the Password is empty");
        g_signal_connect(dialog, "response", G_CALLBACK(gtk_window_close), NULL);
        gtk_widget_show(dialog);
        return;

    }else{
        hash_password(password, hashed_password);

        char path[100];
        User user;
        snprintf(path, sizeof(path), "..\\DataBase\\Users\\user_%s.bin", id);
        FILE *file = fopen(path, "rb");
        if (file == NULL) {
            printf("User not found\n");
            goto end;
        }
        fread(&user, sizeof(User), 1, file);
        fclose(file);
        g_object_set_data(G_OBJECT(main_window), "id", GINT_TO_POINTER(user.user_id));
        if (memcmp(user.password_hashed, hashed_password, 32) == 0) {

            if(strcmp(user.role, "super admin") == 0) {
                Admin_Page(GTK_WIDGET(main_window), user_data);
            } else if(strcmp(user.role, "admin") == 0) {

            } else if(strcmp(user.role, "User") == 0) {
                if (strcmp(user.status , "Active") == 0) {
                    Active_Member_Page(GTK_WIDGET(widget), user_data);
                } else {
                    GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(main_window), GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "%s", "Your Account is not Active. Please contact the Admin");
                    g_signal_connect(dialog, "response", G_CALLBACK(gtk_window_close), NULL);
                    gtk_widget_show(dialog);
                }
            }
        } else {
            end:
                GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(main_window), GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "%s", "The Password or the ID is incorrect");
            g_signal_connect(dialog, "response", G_CALLBACK(gtk_window_close), NULL);
            gtk_widget_show(dialog);
        }
    }


}
