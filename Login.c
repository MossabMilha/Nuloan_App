#include <stdio.h>
#include "Login.h"
#include "Login_Process_Handle_Window.h"
#include "First_Page.h"
#include "Forget_Password.h"

void Login_UI(GtkWidget *widget, gpointer user_data) {
    GtkWidget *main_window = GTK_WIDGET(user_data);

    // Create a new Login window
    GtkWidget *login_window = gtk_window_new();
    gtk_window_set_title(GTK_WINDOW(login_window), "Login");
    gtk_window_set_default_size(GTK_WINDOW(login_window), 350, 600);
    gtk_window_set_resizable(GTK_WINDOW(login_window), FALSE);

    GtkWidget *login_fix = gtk_fixed_new();
    gtk_window_set_child(GTK_WINDOW(login_window), login_fix);

    // Login button provider
    GtkCssProvider *login_provider = gtk_css_provider_new();
    gtk_css_provider_load_from_string(login_provider,
        "button#login_button { background: #FFFFFF; color: #000000; "
        "border-radius: 25px; border: none; }"
    );

    // Login button hover effect provider
    GtkCssProvider *login_hover_provider = gtk_css_provider_new();
    gtk_css_provider_load_from_string(login_hover_provider,
        "button#login_button:hover { background: #E5E5E5; }"
    );

    // Forget Password button provider
    GtkCssProvider *forget_password_provider = gtk_css_provider_new();
    gtk_css_provider_load_from_string(forget_password_provider,
        "button#forget_password { background: #FFFFFF; color: #000000; "
        "border-radius: 25px; border: none; }"
    );

    // Forget Password button hover effect provider
    GtkCssProvider *hover_forget_password_provider = gtk_css_provider_new();
    gtk_css_provider_load_from_string(hover_forget_password_provider,
        "button#forget_password:hover { background: #E5E5E5; }"
    );

    // Back button provider
    GtkCssProvider *back_button_provider = gtk_css_provider_new();
    gtk_css_provider_load_from_string(back_button_provider,
        "button#back_button { background: #FFFFFF; color: #000000; "
        "border-radius: 25px; border: none; }"
    );

    // Back button hover effect provider
    GtkCssProvider *hover_back_button_provider = gtk_css_provider_new();
    gtk_css_provider_load_from_string(hover_back_button_provider,
        "button#back_button:hover { background: #E5E5E5; }"
    );

    // Add transparency to the entry fields
    GtkCssProvider *entry_provider = gtk_css_provider_new();
    gtk_css_provider_load_from_string(entry_provider,
        "entry { "
        "  background-color: rgba(255, 255, 255, 0); "  // Semi-transparent white
        "  border: 1px solid #ddd; "  // Light border for better visibility
        "  border-radius: 25px; "  // Rounded corners for a more modern look
        "  padding: 10px; "  // Padding inside the entry
        "  font-size: 17px; "
        "  color: #FFFFFF;  "
        "} "
    );

    GtkWidget *id_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(id_entry), "Username or ID");
    gtk_fixed_put(GTK_FIXED(login_fix), id_entry, 50, 100);
    gtk_widget_set_size_request(id_entry, 250, 20);
    GtkStyleContext *id_entry_context = gtk_widget_get_style_context(id_entry);
    gtk_style_context_add_provider(id_entry_context, GTK_STYLE_PROVIDER(entry_provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    GtkWidget *password_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(password_entry), "Password");
    gtk_entry_set_visibility(GTK_ENTRY(password_entry), FALSE);
    gtk_entry_set_invisible_char(GTK_ENTRY(password_entry), '*');
    gtk_fixed_put(GTK_FIXED(login_fix), password_entry, 50, 160);
    gtk_widget_set_size_request(password_entry, 250, 20);
    GtkStyleContext *password_entry_context = gtk_widget_get_style_context(password_entry);
    gtk_style_context_add_provider(password_entry_context, GTK_STYLE_PROVIDER(entry_provider), GTK_STYLE_PROVIDER_PRIORITY_USER);


    // Login button
    GtkWidget *login_button = gtk_button_new_with_label("Login");
    gtk_widget_set_name(login_button, "login_button");  // Set ID for CSS to apply
    gtk_fixed_put(GTK_FIXED(login_fix), login_button, 50, 280);
    gtk_widget_set_size_request(login_button, 250, 20);
    GtkStyleContext *login_context = gtk_widget_get_style_context(login_button);
    gtk_style_context_add_provider(login_context, GTK_STYLE_PROVIDER(login_provider),
                                   GTK_STYLE_PROVIDER_PRIORITY_USER);
    gtk_style_context_add_provider(login_context, GTK_STYLE_PROVIDER(login_hover_provider),
                                   GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);  // Higher priority for hover
    g_signal_connect(login_button, "clicked", G_CALLBACK(check_login), login_window);

    // Forget Password button
    GtkWidget *forget_password = gtk_button_new_with_label("Forget Password");
    gtk_widget_set_name(forget_password, "forget_password");  // Set ID for CSS to apply
    gtk_fixed_put(GTK_FIXED(login_fix), forget_password, 50, 320);
    gtk_widget_set_size_request(forget_password, 250, 20);
    GtkStyleContext *forget_password_context = gtk_widget_get_style_context(forget_password);
    gtk_style_context_add_provider(forget_password_context, GTK_STYLE_PROVIDER(forget_password_provider),
                                   GTK_STYLE_PROVIDER_PRIORITY_USER);
    gtk_style_context_add_provider(forget_password_context, GTK_STYLE_PROVIDER(hover_forget_password_provider),
                                   GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);  // Higher priority for hover
    g_signal_connect(forget_password, "clicked", G_CALLBACK(Forget_Password_UI), login_window);

    // Back button
    GtkWidget *back = gtk_button_new_with_label("Back");
    gtk_widget_set_name(back, "back_button");  // Set unique ID for CSS
    gtk_fixed_put(GTK_FIXED(login_fix), back, 50, 360);
    gtk_widget_set_size_request(back, 250, 20);
    GtkStyleContext *back_context = gtk_widget_get_style_context(back);
    gtk_style_context_add_provider(back_context, GTK_STYLE_PROVIDER(back_button_provider),
                                   GTK_STYLE_PROVIDER_PRIORITY_USER);
    gtk_style_context_add_provider(back_context, GTK_STYLE_PROVIDER(hover_back_button_provider),
                                   GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);  // Higher priority for hover
    g_signal_connect(back, "clicked", G_CALLBACK(First_Page_UI), login_window);

    // Store data
    g_object_set_data(G_OBJECT(login_window), "id_entry", id_entry);
    g_object_set_data(G_OBJECT(login_window), "password_entry", password_entry);

    // Show the Login window
    gtk_widget_set_visible(login_window, TRUE);
    gtk_widget_set_visible(main_window, FALSE);
}
