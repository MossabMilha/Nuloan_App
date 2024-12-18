
#include <gtk/gtk.h>
#include "First_Page.h"




static void activate(GtkApplication *app, gpointer user_data) {

    // Create a new window for the application
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Window");
    gtk_window_set_default_size(GTK_WINDOW(window), 350, 600);
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);


    GtkWidget *fix = gtk_fixed_new();


    // Login button
    GtkWidget *Login_Button = gtk_button_new_with_label("Start");
    gtk_fixed_put(GTK_FIXED(fix), Login_Button, 125, 225);
    gtk_widget_set_size_request(Login_Button, 100, 50);

    // Connect the clicked signal of the login button, passing the main window as user_data
    g_signal_connect(Login_Button, "clicked", G_CALLBACK(First_Page_UI), window);


    gtk_window_set_child(GTK_WINDOW(window), fix);

    // Present the main window
    gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char **argv) {
    GtkApplication *app;
    int status;

    app = gtk_application_new("org.gtk.example", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);

    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}