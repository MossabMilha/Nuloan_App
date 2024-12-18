    //
    // Created by PC on 04-Dec-24.
    //
    #include <stdio.h>
    #include <gtk/gtk.h>
    #include "SignUp.h"
    #include "Login.h"


    char global_first_name[50];
    char global_last_name[50];
    char global_cin[10];
    char global_email[50];
    char global_birthday[11];
    char global_password[50];
    int global_2FA_code;
    #include "First_Page.h"


    static gboolean update_rotating_text(gpointer user_data) {
        static int offset = 0;
        GtkLabel *label = GTK_LABEL(user_data);
        const char *text = "MMILHA IS HERE So Just Go womp womp womp     "; // Add some spaces for smooth looping

        int text_length = strlen(text);
        offset = (offset + 1) % text_length; // Calculate the current position of the scrolling text

        char rotated_text[256];
        snprintf(rotated_text, sizeof(rotated_text), "%s%s", text + offset, text);
        gtk_label_set_text(label, rotated_text);

        return TRUE; // Keep the timeout running
    }

    void First_Page_UI(GtkButton *button, gpointer user_data) {
    GtkWidget *main_window = GTK_WIDGET(user_data);

    // Create the main window
    GtkWidget *window = gtk_window_new();
    gtk_window_set_title(GTK_WINDOW(window), "Nuloan");
    gtk_window_set_default_size(GTK_WINDOW(window), 350, 600);
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);


    GtkCssProvider *background_provider = gtk_css_provider_new();
    gtk_css_provider_load_from_string(background_provider,
    "window { "
    "   background: linear-gradient(to right, #002A70, #227593); "
    "   color: #FFFFFF; "
    "}"
);

    // Title CSS provider
    GtkCssProvider *title_provider = gtk_css_provider_new();
    gtk_css_provider_load_from_string(title_provider,
        "label#title_label { color: #FFFFFF; font-size: 30px; font-weight: bold; }"
    );

    // Login button CSS provider
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

    // SignUp button CSS provider
    GtkCssProvider *signup_provider = gtk_css_provider_new();
    gtk_css_provider_load_from_string(signup_provider,
        "button#signup_button { background: #FFFFFF; color: #000000; "
        "border-radius: 25px; border: none; }"
    );

    // SignUp button hover effect provider
    GtkCssProvider *signup_hover_provider = gtk_css_provider_new();
    gtk_css_provider_load_from_string(signup_hover_provider,
        "button#signup_button:hover { background: #E5E5E5; }"
    );


    // ====== APPLY CSS PROVIDERS ======
    // Attach background CSS to the window
    gtk_style_context_add_provider_for_display(gdk_display_get_default(),
        GTK_STYLE_PROVIDER(background_provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    // ====== LAYOUT ======
    GtkWidget *fix = gtk_fixed_new();

    // Title
    GtkWidget *title = gtk_label_new("Welcome to\nNuloan");
    gtk_widget_set_name(title, "title_label");  // Set a name for CSS
    GtkStyleContext *title_context = gtk_widget_get_style_context(title);
    gtk_style_context_add_provider(title_context, GTK_STYLE_PROVIDER(title_provider),
                                   GTK_STYLE_PROVIDER_PRIORITY_USER);
    gtk_fixed_put(GTK_FIXED(fix), title, 100, 100);

    // Login Button
    GtkWidget *login_button = gtk_button_new_with_label("Login");
    gtk_widget_set_name(login_button, "login_button");  // Set a name for CSS
    gtk_widget_set_size_request(login_button, 200, 50);
    GtkStyleContext *login_context = gtk_widget_get_style_context(login_button);
    gtk_style_context_add_provider(login_context, GTK_STYLE_PROVIDER(login_provider),
                                   GTK_STYLE_PROVIDER_PRIORITY_USER);
    gtk_style_context_add_provider(login_context, GTK_STYLE_PROVIDER(login_hover_provider),
                                   GTK_STYLE_PROVIDER_PRIORITY_USER);
    gtk_fixed_put(GTK_FIXED(fix), login_button, 75, 300);

    // Connect the clicked signal
    g_signal_connect(login_button, "clicked", G_CALLBACK(Login_UI), window);

    // SignUp Button
    GtkWidget *signup_button = gtk_button_new_with_label("SignUp");
    gtk_widget_set_name(signup_button, "signup_button");  // Set a name for CSS
    gtk_widget_set_size_request(signup_button, 200, 50);
    GtkStyleContext *signup_context = gtk_widget_get_style_context(signup_button);
    gtk_style_context_add_provider(signup_context, GTK_STYLE_PROVIDER(signup_provider),
                                   GTK_STYLE_PROVIDER_PRIORITY_USER);
    gtk_style_context_add_provider(signup_context, GTK_STYLE_PROVIDER(signup_hover_provider),
                                   GTK_STYLE_PROVIDER_PRIORITY_USER);
    gtk_fixed_put(GTK_FIXED(fix), signup_button, 75, 370);

    // Connect the clicked signal
    g_signal_connect(signup_button, "clicked", G_CALLBACK(Sign_Up_UI), window);

        // Rotating Text Label
        GtkWidget *rotating_label = gtk_label_new("MMILHA IS HERE So Just Go womp womp womp");
    gtk_fixed_put(GTK_FIXED(fix), rotating_label, 10, 550); // Place it at the bottom of the window
    gtk_label_set_width_chars(GTK_LABEL(rotating_label), 40); // Set a max width
    gtk_label_set_ellipsize(GTK_LABEL(rotating_label), PANGO_ELLIPSIZE_END); // Ellipsize if text overflows
    GtkCssProvider *rotating_label_provider = gtk_css_provider_new();
    gtk_css_provider_load_from_string(rotating_label_provider, "label { color: #FFFFFF; }");
    GtkStyleContext *rotating_label_context = gtk_widget_get_style_context(rotating_label);
    gtk_style_context_add_provider(rotating_label_context, GTK_STYLE_PROVIDER(rotating_label_provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

            // Start the rotating text effect
        g_timeout_add(100, update_rotating_text, rotating_label); // Adjust timeout for speed

    // Set the layout
    gtk_window_set_child(GTK_WINDOW(window), fix);

    // Show the new window and hide the main window
    gtk_widget_set_visible(window, TRUE);
    gtk_widget_set_visible(main_window, FALSE);
}




