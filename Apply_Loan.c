// Created by PC on 03-Dec-24.

#include "Apply_Loan.h"
#include "Active_Member.h"
#include "Apply_Loan_More_Information.h"
#include "Structures/Loans_Types.h"

loan_type loan_info_1[6];

void read_loan_info_1() {
    for (int i = 0; i < 6; i++) {
        char path[100];
        snprintf(path, sizeof(path), "..\\DataBase\\Loans\\loan_%d.bin", i + 1);
        FILE *file = fopen(path, "rb");
        if (file == NULL) {
            fprintf(stderr, "Could not open loan_info.bin for reading\n");
            exit(1);
        }
        fread(&loan_info_1[i], sizeof(loan_type), 1, file);
        fclose(file);
    }
}

void Apply_Loan_UI(GtkWidget *widget, gpointer user_data) {
    read_loan_info_1();
    GtkWidget *main_window = GTK_WIDGET(user_data);
    const gint page = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(main_window), "Page"));
    const gint id = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(main_window), "id"));

    // Create a new Apply Loan window
    GtkWidget *Apply_Loan_window = gtk_window_new();
    gtk_window_set_title(GTK_WINDOW(Apply_Loan_window), "Apply To Loan");
    gtk_window_set_default_size(GTK_WINDOW(Apply_Loan_window), 460, 680);
    gtk_window_set_resizable(GTK_WINDOW(Apply_Loan_window), FALSE);
    g_object_set_data(G_OBJECT(Apply_Loan_window), "id", GINT_TO_POINTER(id));
    g_object_set_data(G_OBJECT(Apply_Loan_window), "id", GINT_TO_POINTER(id));

    // Define CSS for buttons
    GtkCssProvider *button_provider = gtk_css_provider_new();
    gtk_css_provider_load_from_string(button_provider,
        "button { "
        "   background: linear-gradient(to bottom, #D3D3D3, #B0B0B0); " /* Light gray to darker gray */
        "   color: #2C2C2C; " /* Darker text color for contrast */
        "   font-size: 15px; "
        "   font-weight: bold; "
        "   border: 1px solid #A9A9A9; "
        "   border-radius: 10px; "
        "   padding: 8px 12px; "
        "   margin: 5px; "
        "} "
        "button:hover { "
        "   background: #C0C0C0; " /* Slightly lighter gray */
        "} "
        "button:active { "
        "   background: #A0A0A0; " /* Darker gray when pressed */
        "} ");

    GtkCssProvider *frame_provider = gtk_css_provider_new();
    gtk_css_provider_load_from_string(frame_provider,
        "frame { "
        "   border: 1px solid #A9A9A9; "
        "   border-radius: 10px; "
        "   background-color: rgba(10, 38, 71, 0.3); " /* Light gray background */
        "   margin: 10px; "
        "   padding: 10px; "
        "}");

    GtkCssProvider *label_provider = gtk_css_provider_new();
    gtk_css_provider_load_from_string(label_provider,
        "label { "
        "   color: #FFFFFF; "
        "   font-family: Arial; "
        "   font-size: 17px; "
        "}");

    GtkWidget *Apply_Loan_fix = gtk_fixed_new();
    gtk_window_set_child(GTK_WINDOW(Apply_Loan_window), Apply_Loan_fix);

    // Buttons
    GtkWidget *next_button = gtk_button_new_with_label("Next");
    gtk_widget_set_size_request(next_button, 80, 30);
    gtk_fixed_put(GTK_FIXED(Apply_Loan_fix), next_button, 350, 620);
    g_signal_connect(next_button, "clicked", G_CALLBACK(Apply_Loan_UI), Apply_Loan_window);

    GtkWidget *previous_button = gtk_button_new_with_label("Previous");
    gtk_widget_set_size_request(previous_button, 80, 30);
    gtk_fixed_put(GTK_FIXED(Apply_Loan_fix), previous_button, 25, 620);
    g_signal_connect(previous_button, "clicked", G_CALLBACK(Apply_Loan_UI), Apply_Loan_window);

    GtkWidget *back_button = gtk_button_new_with_label("Exit");
    gtk_widget_set_size_request(back_button, 80, 30);
    gtk_fixed_put(GTK_FIXED(Apply_Loan_fix), back_button, 155, 620);
    g_signal_connect(back_button, "clicked", G_CALLBACK(Active_Member_Page), Apply_Loan_window);

    GtkStyleContext *next_context = gtk_widget_get_style_context(next_button);
    gtk_style_context_add_provider(next_context, GTK_STYLE_PROVIDER(button_provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    GtkStyleContext *previous_context = gtk_widget_get_style_context(previous_button);
    gtk_style_context_add_provider(previous_context, GTK_STYLE_PROVIDER(button_provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    GtkStyleContext *back_context = gtk_widget_get_style_context(back_button);
    gtk_style_context_add_provider(back_context, GTK_STYLE_PROVIDER(button_provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    gtk_widget_set_visible(next_button, FALSE);
    gtk_widget_set_visible(previous_button, FALSE);

    if (page == 1) {
        gtk_widget_set_visible(next_button, TRUE);
        g_object_set_data(G_OBJECT(Apply_Loan_window), "Page", GINT_TO_POINTER(2));

        for (int i = 0; i < 4; i++) {
            gpointer *tab = g_malloc(sizeof(gpointer) * 3); // Allocate space for 3 items
            tab[0] = GINT_TO_POINTER(id);                  // Store id as a pointer
            tab[1] = GINT_TO_POINTER(i);               // Store index as a pointer
            tab[2] = Apply_Loan_window;                    // Store GtkWidget* directly

            // Create a frame with a fixed size
            GtkWidget *frame = gtk_frame_new(NULL);
            gtk_widget_set_size_request(frame, 400, 120); // Fixed frame size
            GtkStyleContext *frame_context = gtk_widget_get_style_context(frame);
            gtk_style_context_add_provider(frame_context, GTK_STYLE_PROVIDER(frame_provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

            // Create a label for the loan information
            GtkWidget *user_id = gtk_label_new("");
            char id_text[1000];
            sprintf(id_text, "<span weight='bold'>TITLE:</span> %s\n%s", loan_info_1[i].loan_title, loan_info_1[i].loan_one_line_description);
            gtk_label_set_markup(GTK_LABEL(user_id), id_text);
            gtk_label_set_xalign(GTK_LABEL(user_id), 0.1); // Align text within the label
            gtk_widget_set_size_request(user_id, 380, 60); // Ensure label fits well within the frame

            GtkStyleContext *user_context = gtk_widget_get_style_context(user_id);
            gtk_style_context_add_provider(user_context, GTK_STYLE_PROVIDER(label_provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

            // Create a button for more information
            GtkWidget *more_button = gtk_button_new_with_label("More Information");
            gtk_widget_set_size_request(more_button, 290, 30); // Fixed button size
            printf("=1==%d\n", GPOINTER_TO_INT(tab[1]));
            g_signal_connect(more_button, "clicked", G_CALLBACK(Apply_Loan_Page_2_UI), tab);
            GtkStyleContext *more_context = gtk_widget_get_style_context(more_button);
            gtk_style_context_add_provider(more_context, GTK_STYLE_PROVIDER(button_provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

            // Arrange label and button inside a vertical box
            GtkWidget *frame_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
            gtk_box_append(GTK_BOX(frame_box), user_id);
            gtk_box_append(GTK_BOX(frame_box), more_button);

            // Add the box to the frame and place the frame in the fixed layout
            gtk_frame_set_child(GTK_FRAME(frame), frame_box);
            gtk_fixed_put(GTK_FIXED(Apply_Loan_fix), frame, 25, 20 + 150 * i); // Position with consistent spacing
        }
    } else {
        g_object_set_data(G_OBJECT(Apply_Loan_window), "Page", GINT_TO_POINTER(1));
        gtk_widget_set_visible(previous_button, TRUE);

        for (int i = 4; i < 6; i++) {
            gpointer *tab = g_malloc(sizeof(gpointer) * 3); // Allocate space for 3 items
            tab[0] = GINT_TO_POINTER(id);                  // Store id as a pointer
            tab[1] = GINT_TO_POINTER(i + 1);               // Store index as a pointer
            tab[2] = Apply_Loan_window;                    // Store GtkWidget* directly

            // Create a frame with a fixed size
            GtkWidget *frame = gtk_frame_new(NULL);
            gtk_widget_set_size_request(frame, 400, 120); // Fixed frame size
            GtkStyleContext *frame_context = gtk_widget_get_style_context(frame);
            gtk_style_context_add_provider(frame_context, GTK_STYLE_PROVIDER(frame_provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

            // Create a label for the loan information
            GtkWidget *user_id = gtk_label_new("");
            char id_text[1000];
            sprintf(id_text, "<span weight='bold'>TITLE:</span> %s\n%s", loan_info_1[i].loan_title, loan_info_1[i].loan_one_line_description);
            gtk_label_set_markup(GTK_LABEL(user_id), id_text);
            gtk_label_set_xalign(GTK_LABEL(user_id), 0.1); // Align text within the label
            gtk_widget_set_size_request(user_id, 380, 60); // Ensure label fits well within the frame

            GtkStyleContext *user_context = gtk_widget_get_style_context(user_id);
            gtk_style_context_add_provider(user_context, GTK_STYLE_PROVIDER(label_provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

            // Create a button for more information
            GtkWidget *more_button = gtk_button_new_with_label("More Information");
            gtk_widget_set_size_request(more_button, 290, 30); // Fixed button size
            g_signal_connect(more_button, "clicked", G_CALLBACK(Apply_Loan_Page_2_UI), tab);
            GtkStyleContext *more_context = gtk_widget_get_style_context(more_button);
            gtk_style_context_add_provider(more_context, GTK_STYLE_PROVIDER(button_provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

            // Arrange label and button inside a vertical box
            GtkWidget *frame_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
            gtk_box_append(GTK_BOX(frame_box), user_id);
            gtk_box_append(GTK_BOX(frame_box), more_button);

            // Add the box to the frame and place the frame in the fixed layout
            gtk_frame_set_child(GTK_FRAME(frame), frame_box);
            gtk_fixed_put(GTK_FIXED(Apply_Loan_fix), frame, 25, 20 + 150 * (i-4));
        }
    }

    gtk_widget_set_visible(Apply_Loan_window, TRUE);
    gtk_widget_set_visible(main_window, FALSE);
}
