#include "Apply_Loan_More_Information.h"
#include "Apply_Loan.h"
#include "Apply_Loan_fianl.h"
#include "Structures/Loans_Types.h"
#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>

loan_type loan_info[6];

// Function to read loan information from binary files
void read_loan_info_2() {
    for (int i = 0; i < 6; i++) {
        char path[100];
        snprintf(path, sizeof(path), "..\\DataBase\\Loans\\loan_%d.bin", i + 1);
        FILE *file = fopen(path, "rb");
        if (file == NULL) {
            fprintf(stderr, "Could not open loan_%d.bin for reading\n", i + 1);
            exit(1);
        }
        fread(&loan_info[i], sizeof(loan_type), 1, file);
        fclose(file);
    }
}

// Function to display the Apply Loan Page 2 UI
void Apply_Loan_Page_2_UI(GtkWidget *widget, gpointer user_data) {
    read_loan_info_2();
    gpointer *tab = (gpointer *)user_data;
    int id = GPOINTER_TO_INT(tab[0]);
    int i = GPOINTER_TO_INT(tab[1]);
    GtkWidget *window = (GtkWidget *)tab[2];

    // Check for valid index
    if (i < 0 || i >= 6) {
        fprintf(stderr, "Invalid loan index: %d\n", i);
        return;
    }

    // Create a new window
    GtkWidget *Apply_Loan_Page2_window = gtk_window_new();
    gtk_window_set_title(GTK_WINDOW(Apply_Loan_Page2_window), "Apply To Loan");
    gtk_window_set_default_size(GTK_WINDOW(Apply_Loan_Page2_window), 350, 650);
    gtk_window_set_resizable(GTK_WINDOW(Apply_Loan_Page2_window), FALSE);

    GtkWidget *Apply_Loan_fix = gtk_fixed_new();
    gtk_window_set_child(GTK_WINDOW(Apply_Loan_Page2_window), Apply_Loan_fix);

    // Add Loan Title
    GtkWidget *label = gtk_label_new("");
    char text[500];
    sprintf(text, "<span font='15' weight='bold' foreground='#ffffff'>%s</span>", loan_info[i].loan_title);
    gtk_label_set_markup(GTK_LABEL(label), text);
    gtk_fixed_put(GTK_FIXED(Apply_Loan_fix), label, 120, 20);


    // Create a frame for loan information
    GtkWidget *frame = gtk_frame_new(NULL);
    gtk_widget_set_size_request(frame, 300, 300);
    gtk_fixed_put(GTK_FIXED(Apply_Loan_fix), frame, 25, 80);

    // Custom label for the frame
    GtkWidget *frame_label = gtk_label_new(NULL);
    char styled_label[100];
    sprintf(styled_label, "<span font='12' weight='bold' foreground='#ffffff'>Loan Information</span>");
    gtk_label_set_markup(GTK_LABEL(frame_label), styled_label);

    // Set the custom label to the frame
    gtk_frame_set_label_widget(GTK_FRAME(frame), frame_label);
    gtk_frame_set_label_align(GTK_FRAME(frame), 0.5); // Center align

    // Box for loan details
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_widget_set_margin_start(box, 10);
    gtk_widget_set_margin_end(box, 10);
    gtk_widget_set_margin_top(box, 10);
    gtk_widget_set_margin_bottom(box, 10);
    gtk_frame_set_child(GTK_FRAME(frame), box);

    // Add Loan Details
    GtkWidget *min_amount_label = gtk_label_new("");
    sprintf(text, "<span font='12' foreground='#ffffff'>• Min Amount: %.2f</span>", loan_info[i].min_amount);
    gtk_label_set_markup(GTK_LABEL(min_amount_label), text);
    gtk_label_set_xalign(GTK_LABEL(min_amount_label), 0);
    gtk_box_append(GTK_BOX(box), min_amount_label);

    GtkWidget *max_amount_label = gtk_label_new("");
    sprintf(text, "<span font='12' foreground='#ffffff'>• Max Amount: %.2f</span>", loan_info[i].max_amount);
    gtk_label_set_markup(GTK_LABEL(max_amount_label), text);
    gtk_label_set_xalign(GTK_LABEL(max_amount_label), 0);
    gtk_box_append(GTK_BOX(box), max_amount_label);

    GtkWidget *min_duration_label = gtk_label_new("");
    sprintf(text, "<span font='12' foreground='#ffffff'>• Min Duration: %d</span>", loan_info[i].min_duration);
    gtk_label_set_markup(GTK_LABEL(min_duration_label), text);
    gtk_label_set_xalign(GTK_LABEL(min_duration_label), 0);
    gtk_box_append(GTK_BOX(box), min_duration_label);

    GtkWidget *interest_label = gtk_label_new("");
    sprintf(text, "<span font='12' foreground='#ffffff'>• Interest: %.2f</span>", *loan_info[i].interest_rate);
    gtk_label_set_markup(GTK_LABEL(interest_label), text);
    gtk_label_set_xalign(GTK_LABEL(interest_label), 0);
    gtk_box_append(GTK_BOX(box), interest_label);

    // Add Description
    GtkWidget *description_label = gtk_label_new("");
    sprintf(text, "<span font='12' foreground='#ffffff'>• Description: %s</span>", loan_info[i].loan_description);
    gtk_label_set_markup(GTK_LABEL(description_label), text);
    gtk_label_set_wrap(GTK_LABEL(description_label), TRUE);
    gtk_label_set_xalign(GTK_LABEL(description_label), 0);
    gtk_box_append(GTK_BOX(box), description_label);

    // Apply Button
    GtkWidget *apply_button = gtk_button_new_with_label("Apply");
    gtk_widget_set_size_request(apply_button, 300, 30);
    gtk_fixed_put(GTK_FIXED(Apply_Loan_fix), apply_button, 25, 470);
    g_object_set_data(G_OBJECT(Apply_Loan_Page2_window), "user_id", GINT_TO_POINTER(id));
    g_object_set_data(G_OBJECT(Apply_Loan_Page2_window), "loan_id", GINT_TO_POINTER(i));
    g_signal_connect(apply_button, "clicked", G_CALLBACK(Final_Apply), Apply_Loan_Page2_window);

    // Back Button
    GtkWidget *Back_button = gtk_button_new_with_label("Go Back");
    gtk_widget_set_size_request(Back_button, 300, 30);
    gtk_fixed_put(GTK_FIXED(Apply_Loan_fix), Back_button, 25, 520);
    g_signal_connect(Back_button, "clicked", G_CALLBACK(Apply_Loan_UI), Apply_Loan_Page2_window);
    g_object_set_data(G_OBJECT(Apply_Loan_Page2_window), "Page", GINT_TO_POINTER(1));

    // Show updated window and hide previous ones
    gtk_widget_set_visible(Apply_Loan_Page2_window, TRUE);
    gtk_widget_set_visible(widget, FALSE);
    gtk_window_close(GTK_WINDOW(window));

}
