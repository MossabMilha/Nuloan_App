#include <stdio.h>
#include <gtk/gtk.h>
#include "Show_Users.h"
#include "Show_Loan_Application.h"
#include "Auto_Check_application.h"
#include "First_Page.h"

void Admin_Page(GtkWidget *widget, gpointer user_data) {
    GtkWidget *main_window = GTK_WIDGET(user_data);

    // Create a new Signup window
    GtkWidget *Admin_Page = gtk_window_new();
    gtk_window_set_title(GTK_WINDOW(Admin_Page), "Admin Page");
    gtk_window_set_default_size(GTK_WINDOW(Admin_Page), 1800, 700);
    gtk_window_set_resizable(GTK_WINDOW(Admin_Page), FALSE);

    // Create a fixed layout for the Bank Information window
    GtkWidget *Admin_Page_Fix = gtk_fixed_new();
    gtk_widget_set_size_request(Admin_Page_Fix, 400, 600);

    // Create a fixed layout for the Admin OutPut
    GtkWidget *Admin_OutPut_Fix = gtk_fixed_new();
    gtk_widget_set_size_request(Admin_OutPut_Fix, 120, 600);

    // Create Show Users_information button
    GtkWidget *Show_User = gtk_button_new_with_label("Show Users");
    gtk_fixed_put(GTK_FIXED(Admin_Page_Fix), Show_User, 75, 100);
    gtk_widget_set_size_request(Show_User, 250, 20);

    // Create Show Users_information button
    GtkWidget *Show_Loan_application = gtk_button_new_with_label("Show Loan Application");
    gtk_fixed_put(GTK_FIXED(Admin_Page_Fix), Show_Loan_application, 75, 160);
    gtk_widget_set_size_request(Show_Loan_application, 250, 20);



    // Create Show Users_information button
    GtkWidget *Automatic_Check_app = gtk_button_new_with_label("Automatic Check application");
    gtk_fixed_put(GTK_FIXED(Admin_Page_Fix), Automatic_Check_app, 75, 220);
    gtk_widget_set_size_request(Automatic_Check_app, 250, 20);



    // Create Log Out button
    GtkWidget *log_out = gtk_button_new_with_label("Log Out");
    gtk_fixed_put(GTK_FIXED(Admin_Page_Fix), log_out, 75, 450);
    gtk_widget_set_size_request(log_out, 250, 20);





    // Add the fixed layouts to a container
    GtkWidget *container = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_box_append(GTK_BOX(container), Admin_Page_Fix);
    gtk_box_append(GTK_BOX(container), Admin_OutPut_Fix);
    gtk_window_set_child(GTK_WINDOW(Admin_Page), container);


    g_object_set_data(G_OBJECT(Admin_Page), "Show_User", Show_User);
    g_object_set_data(G_OBJECT(Admin_Page), "Admin_Page_Fix", Admin_Page_Fix);
    g_object_set_data(G_OBJECT(Admin_Page), "Admin_OutPut_Fix", Admin_OutPut_Fix);
    g_object_set_data(G_OBJECT(Admin_Page), "Page", GINT_TO_POINTER(1));
    g_object_set_data(G_OBJECT(Admin_Page), "logout", log_out);

    g_signal_connect(Show_User, "clicked", G_CALLBACK(Check_User), Admin_Page);
    g_signal_connect(Show_Loan_application, "clicked", G_CALLBACK(Check_Applications), Admin_Page);
    g_signal_connect(Automatic_Check_app, "clicked", G_CALLBACK(Auto_Check_app), Admin_Page);
    g_signal_connect(log_out, "clicked", G_CALLBACK(First_Page_UI), Admin_Page);

    // Show all widgets
    gtk_window_present(GTK_WINDOW(Admin_Page));

    gtk_widget_set_visible(Admin_Page, TRUE);
    gtk_widget_set_visible(main_window, FALSE);
}
