#include <stdio.h>
#include <gtk/gtk.h>
#include "SignUp.h"
#include "Check_Signup_Information.h"
#include "First_Page.h"
void Sign_Up_UI(GtkButton *button, gpointer user_data) {
    GtkWidget *main_window = GTK_WIDGET(user_data);

    // Create a new Signup window
    GtkWidget *signup_window = gtk_window_new();
    gtk_window_set_title(GTK_WINDOW(signup_window), "SignUp");
    gtk_window_set_default_size(GTK_WINDOW(signup_window), 350, 600);
    gtk_window_set_resizable(GTK_WINDOW(signup_window), FALSE);

    // Create a fixed layout for the signup window
    GtkWidget *signup_fix = gtk_fixed_new();

    // Create a CSS provider
    GtkCssProvider *entry_provider = gtk_css_provider_new();
    gtk_css_provider_load_from_string(entry_provider,
    "entry { "
    "  background-color: rgba(255, 255, 255, 0); "  // Semi-transparent white
    "  border: 1px solid #ffffff; "  // Light border for better visibility
    "  border-radius: 25px; "  // Adjust corner roundness
    "  padding: 5px; "  // Reduce padding
    "  min-height: 20px; "  // Enforce smaller height
    "  min-width: 200px; "  // Enforce width
    "  font-size: 15px; "
    "  color: #ffffff;  "
    "} "
    );
    // Login button CSS provider
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
        "}"
    );

    GtkCssProvider *back_button_provider = gtk_css_provider_new();
    gtk_css_provider_load_from_string(back_button_provider,
        "button#back_button { "
        "   background: #FFFFFF; "
        "   color: #000000; "
        "   border-radius: 25px; "
        "   border: none; "
        "} "
        "button#back_button:hover { "
        "   background: #FFCCCC; "
        "}"
        );

    GtkCssProvider *image_provider = gtk_css_provider_new();
    gtk_css_provider_load_from_string(image_provider,
        "window { "
        "   background-color: rgba(0, 0, 0, 0); "  // Transparent background for the window
        "} "
        "button { "
        "   border-radius: 25px; "
        "   background-color: rgba(255, 255, 255, 0.1); "  // Transparent white background
        "   color: white; "
        "   font-size: 16px; "
        "   border: none; "
        "   transition: background-color 0.3s; "
        "} "
        "button:hover { "
        "   background-color: #45a049; "
        "} "
        "button:active { "
        "   background-color: #388e3c; "
        "} "
    );
    // gtk_style_context_add_provider_for_screen(gdk_screen_get_default(), GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);



    // Create a CSS provider for First Name entry ===============================================================================================
    GtkWidget *first_name = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(first_name), "First Name");
    gtk_fixed_put(GTK_FIXED(signup_fix), first_name, 50, 100);
    gtk_widget_set_size_request(first_name, 250, 20);
    GtkStyleContext *first_name_context = gtk_widget_get_style_context(first_name);
    gtk_style_context_add_provider(first_name_context, GTK_STYLE_PROVIDER(entry_provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    // First name image
    GtkWidget *first_name_button = gtk_button_new();
    GtkWidget *first_name_image = gtk_image_new_from_file("..\\images\\info_red.png");
    gtk_button_set_child(GTK_BUTTON(first_name_button), first_name_image);
    gtk_fixed_put(GTK_FIXED(signup_fix), first_name_button, 310, 100);
    gtk_widget_set_size_request(first_name_button, 20, 20);
    GtkStyleContext *first_name_image_context = gtk_widget_get_style_context(first_name_button);
    gtk_style_context_add_provider(first_name_image_context, GTK_STYLE_PROVIDER(image_provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    gtk_widget_set_visible(first_name_button, FALSE);
    //============================================================================================================================================

    // Create a CSS provider for Last Name entry ===============================================================================================
    GtkWidget *Last_name = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(Last_name), "Last Name");
    gtk_fixed_put(GTK_FIXED(signup_fix), Last_name, 50, 140);
    gtk_widget_set_size_request(Last_name, 250, 25);
    GtkStyleContext *Last_name_context = gtk_widget_get_style_context(Last_name);
    gtk_style_context_add_provider(Last_name_context, GTK_STYLE_PROVIDER(entry_provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    // Last name image
    GtkWidget *last_name_button = gtk_button_new();
    GtkWidget *last_name_image = gtk_image_new_from_file("..\\images\\info_red.png");
    GtkWidget *last_name_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_append(GTK_BOX(last_name_hbox), last_name_image);
    gtk_button_set_child(GTK_BUTTON(last_name_button), last_name_hbox);
    gtk_fixed_put(GTK_FIXED(signup_fix), last_name_button, 310, 140);
    gtk_widget_set_size_request(last_name_button, 20, 20);
    GtkStyleContext *last_name_image_context = gtk_widget_get_style_context(last_name_button);
    gtk_style_context_add_provider(last_name_image_context, GTK_STYLE_PROVIDER(image_provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    gtk_widget_set_visible(last_name_button, FALSE);

    //============================================================================================================================================

    // Create a CSS provider for CIN entry ===============================================================================================
    GtkWidget *cin = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(cin), "CIN");
    gtk_fixed_put(GTK_FIXED(signup_fix), cin, 50, 180);
    gtk_widget_set_size_request(cin, 250, 25);
    GtkStyleContext *cin_context = gtk_widget_get_style_context(cin);
    gtk_style_context_add_provider(cin_context, GTK_STYLE_PROVIDER(entry_provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    // CIN image
    GtkWidget *cin_button = gtk_button_new();
    GtkWidget *cin_image = gtk_image_new_from_file("..\\images\\info_red.png");
    GtkWidget *cin_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_append(GTK_BOX(cin_hbox), cin_image);
    gtk_button_set_child(GTK_BUTTON(cin_button), cin_hbox);
    gtk_fixed_put(GTK_FIXED(signup_fix), cin_button, 310, 180);
    gtk_widget_set_size_request(cin_button, 20, 20);
    GtkStyleContext *cin_image_context = gtk_widget_get_style_context(cin_button);
    gtk_style_context_add_provider(cin_image_context, GTK_STYLE_PROVIDER(image_provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    gtk_widget_set_visible(cin_button, FALSE);

    //============================================================================================================================================

    // Create a CSS provider for Email entry ===============================================================================================
    GtkWidget *Email = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(Email), "Email");
    gtk_fixed_put(GTK_FIXED(signup_fix), Email, 50, 220);
    gtk_widget_set_size_request(Email, 250, 25);
    GtkStyleContext *Email_context = gtk_widget_get_style_context(Email);
    gtk_style_context_add_provider(Email_context, GTK_STYLE_PROVIDER(entry_provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    // Email image
    GtkWidget *email_button = gtk_button_new();
    GtkWidget *email_image = gtk_image_new_from_file("..\\images\\info_red.png");
    GtkWidget *email_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_append(GTK_BOX(email_hbox), email_image);
    gtk_button_set_child(GTK_BUTTON(email_button), email_hbox);
    gtk_fixed_put(GTK_FIXED(signup_fix), email_button, 310, 220);
    gtk_widget_set_size_request(email_button, 20, 20);
    GtkStyleContext *email_image_context = gtk_widget_get_style_context(email_button);
    gtk_style_context_add_provider(email_image_context, GTK_STYLE_PROVIDER(image_provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    gtk_widget_set_visible(email_button, FALSE);
    //============================================================================================================================================

    // Create a CSS provider for Birthday entry ===============================================================================================
    GtkWidget *Birthday = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(Birthday), "Birthday DD//MM//YYY");
    gtk_fixed_put(GTK_FIXED(signup_fix), Birthday, 50, 260);
    gtk_widget_set_size_request(Birthday, 250, 25);
    GtkStyleContext *Birthday_context = gtk_widget_get_style_context(Birthday);
    gtk_style_context_add_provider(Birthday_context, GTK_STYLE_PROVIDER(entry_provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    // Birthday image
    GtkWidget *Birthday_button = gtk_button_new();
    GtkWidget *Birthday_image = gtk_image_new_from_file("..\\images\\info_red.png");
    GtkWidget *Birthday_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_append(GTK_BOX(Birthday_hbox), Birthday_image);
    gtk_button_set_child(GTK_BUTTON(Birthday_button), Birthday_hbox);
    gtk_fixed_put(GTK_FIXED(signup_fix), Birthday_button, 310, 260);
    gtk_widget_set_size_request(Birthday_button, 20, 20);
    GtkStyleContext *Birthday_image_context = gtk_widget_get_style_context(Birthday_button);
    gtk_style_context_add_provider(Birthday_image_context, GTK_STYLE_PROVIDER(image_provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    gtk_widget_set_visible(Birthday_button, FALSE);
    //============================================================================================================================================

    // Create a CSS provider for Password entry ===============================================================================================
    GtkWidget *Password = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(Password), "Password");
    gtk_fixed_put(GTK_FIXED(signup_fix), Password, 50, 300);
    gtk_widget_set_size_request(Password, 250, 25);
    gtk_entry_set_visibility(GTK_ENTRY(Password), FALSE);
    gtk_entry_set_invisible_char(GTK_ENTRY(Password), '*');
    GtkStyleContext *Password_context = gtk_widget_get_style_context(Password);
    gtk_style_context_add_provider(Password_context, GTK_STYLE_PROVIDER(entry_provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    // Password image
    GtkWidget *Password_button = gtk_button_new();
    GtkWidget *Password_image = gtk_image_new_from_file("..\\images\\info_red.png");
    GtkWidget *Password_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_append(GTK_BOX(Password_hbox), Password_image);
    gtk_button_set_child(GTK_BUTTON(Password_button), Password_hbox);
    gtk_fixed_put(GTK_FIXED(signup_fix), Password_button, 310, 300);
    gtk_widget_set_size_request(Password_button, 20, 20);
    GtkStyleContext *Password_image_context = gtk_widget_get_style_context(Password_button);
    gtk_style_context_add_provider(Password_image_context, GTK_STYLE_PROVIDER(image_provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    gtk_widget_set_visible(Password_button, FALSE);

    // Password confirmation entry
    GtkWidget *Password_Confirmation = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(Password_Confirmation), "Confirm Password");
    gtk_fixed_put(GTK_FIXED(signup_fix), Password_Confirmation, 50, 340);
    gtk_widget_set_size_request(Password_Confirmation, 250, 25);
    gtk_entry_set_visibility(GTK_ENTRY(Password_Confirmation), FALSE);
    gtk_entry_set_invisible_char(GTK_ENTRY(Password_Confirmation), '*');
    GtkStyleContext *Password_Confirmation_context = gtk_widget_get_style_context(Password_Confirmation);
    gtk_style_context_add_provider(Password_Confirmation_context, GTK_STYLE_PROVIDER(entry_provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    //============================================================================================================================================

    // Store the entry widgets in the signup window using g_object_set_data
    g_object_set_data(G_OBJECT(signup_window), "signup_fix", signup_fix);
    g_object_set_data(G_OBJECT(signup_window), "First Name", first_name);
    g_object_set_data(G_OBJECT(signup_window), "Last Name", Last_name);
    g_object_set_data(G_OBJECT(signup_window), "CIN", cin);
    g_object_set_data(G_OBJECT(signup_window), "Email", Email);
    g_object_set_data(G_OBJECT(signup_window), "Birthday DD//MM//YYY", Birthday);
    g_object_set_data(G_OBJECT(signup_window), "Password", Password);
    g_object_set_data(G_OBJECT(signup_window), "Confirm Password", Password_Confirmation);
    g_object_set_data(G_OBJECT(signup_window), "First Name Image", first_name_button);
    g_object_set_data(G_OBJECT(signup_window), "Last Name Image", last_name_button);
    g_object_set_data(G_OBJECT(signup_window), "CIN Image", cin_button);
    g_object_set_data(G_OBJECT(signup_window), "Email Image", email_button);
    g_object_set_data(G_OBJECT(signup_window), "Birthday Image", Birthday_button);
    g_object_set_data(G_OBJECT(signup_window), "Password Image", Password_button);

    // Create Submit Button
    GtkWidget *Submit = gtk_button_new_with_label("Submit");
    gtk_widget_set_size_request(Submit, 250, 20);
    gtk_widget_set_name(Submit, "login_button");
    GtkStyleContext *Submit_context = gtk_widget_get_style_context(Submit);
    gtk_style_context_add_provider(Submit_context, GTK_STYLE_PROVIDER(button_provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    gtk_fixed_put(GTK_FIXED(signup_fix), Submit, 50, 380);
    g_signal_connect(Submit, "clicked", G_CALLBACK(check_SignUp), signup_window);

    // Create Back button
    GtkWidget *Back = gtk_button_new_with_label("Back");
    gtk_widget_set_size_request(Back, 250, 20);
    gtk_widget_set_name(Back, "back_button");
    GtkStyleContext *Back_context = gtk_widget_get_style_context(Back);
    gtk_style_context_add_provider(Back_context, GTK_STYLE_PROVIDER(back_button_provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    gtk_fixed_put(GTK_FIXED(signup_fix), Back, 50, 420);
    g_signal_connect(Back, "clicked", G_CALLBACK(First_Page_UI), signup_window);

    // Set the fixed layout as the child of the signup window
    gtk_window_set_child(GTK_WINDOW(signup_window), signup_fix);

    // Show the signup window
    gtk_widget_show(signup_window);

    // Close the main window
    gtk_widget_hide(main_window);
}