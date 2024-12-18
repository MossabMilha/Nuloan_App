#include <stdio.h>
#include <gtk/gtk.h>
#include "Active_Member.h"
#include "Apply_Loan.h"
#include "Check_Loan_Status.h"
#include "First_Page.h"
#include "Structures/User.h"





void Active_Member_Page(GtkWidget *widget, gpointer user_data){
    GtkWidget *main_window = GTK_WIDGET(user_data);
    const gint id = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(main_window), "id"));

    // Create a new Signup window
    GtkWidget *Home_Page = gtk_window_new();
    gtk_window_set_title(GTK_WINDOW(Home_Page), "Home Page");
    gtk_window_set_default_size(GTK_WINDOW(Home_Page), 350, 600);
    gtk_window_set_resizable(GTK_WINDOW(Home_Page), FALSE);
    g_object_set_data(G_OBJECT(Home_Page), "id", GINT_TO_POINTER(id));
    g_object_set_data(G_OBJECT(Home_Page), "id", GINT_TO_POINTER(id));

    GtkCssProvider *label_provider = gtk_css_provider_new();
    gtk_css_provider_load_from_string(label_provider,
        "label, button { "
        "   color: #FFFFFF; "
        "   font-family: Arial; "
        "   font-size: 15px; "
        "}");

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


    User user;
    char path[100];
    snprintf(path, sizeof(path), "..\\DataBase\\Users\\user_%d.bin", id);
    printf("%d\n", id);
    FILE *file = fopen(path, "rb");
    if (file == NULL) {
        printf("User not found\n");
        return;
    }
    fread(&user, sizeof(User), 1, file);
    fclose(file);

    // Create a fixed layout for the Bank Information window
    GtkWidget *fix = gtk_fixed_new();

    GtkWidget *image = gtk_image_new_from_file("..\\Images\\TheLegend.png");
    gtk_image_set_pixel_size(GTK_IMAGE(image), 110);
    gtk_fixed_put(GTK_FIXED(fix), image, 20, 25);

    char Full_Name_text[100];
    sprintf(Full_Name_text, "First Name: %s %s", user.first_name, user.last_name);
    GtkWidget *Full_Name = gtk_label_new(Full_Name_text);
    gtk_fixed_put(GTK_FIXED(fix), Full_Name, 130, 45);
    GtkStyleContext *Full_Name_context = gtk_widget_get_style_context(Full_Name);
    gtk_style_context_add_provider(Full_Name_context, GTK_STYLE_PROVIDER(label_provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    char ID_text[100];
    sprintf(ID_text, "ID : %d", user.user_id);
    GtkWidget *ID = gtk_label_new(ID_text);
    gtk_fixed_put(GTK_FIXED(fix), ID, 130, 65);
    GtkStyleContext *ID_context = gtk_widget_get_style_context(ID); // Corrected context
    gtk_style_context_add_provider(ID_context, GTK_STYLE_PROVIDER(label_provider), GTK_STYLE_PROVIDER_PRIORITY_USER);


    GtkWidget *CIN = gtk_label_new("");
    char CIN_text[100];
    sprintf(CIN_text, "CIN : %s", user.cin);
    gtk_label_set_markup(GTK_LABEL(CIN), CIN_text);
    gtk_fixed_put(GTK_FIXED(fix), CIN, 130, 85);
    GtkStyleContext *CIN_context = gtk_widget_get_style_context(CIN);
    gtk_style_context_add_provider(CIN_context, GTK_STYLE_PROVIDER(label_provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    GtkWidget *Email = gtk_label_new("");
    char Email_text[100];
    sprintf(Email_text, "Email : %s", user.email);
    gtk_label_set_markup(GTK_LABEL(Email), Email_text);
    gtk_fixed_put(GTK_FIXED(fix), Email, 130, 105);
    GtkStyleContext *Email_context = gtk_widget_get_style_context(Email);
    gtk_style_context_add_provider(Email_context, GTK_STYLE_PROVIDER(label_provider), GTK_STYLE_PROVIDER_PRIORITY_USER);




    // GtkWidget *Edit_Information = gtk_button_new_with_label("Edit Information");
    // gtk_fixed_put(GTK_FIXED(fix), Edit_Information, 25, 180);
    // gtk_widget_set_size_request(Edit_Information, 300, 20);
    // gtk_widget_set_name(Edit_Information, "login_button");
    // GtkStyleContext *Edit_Information_context = gtk_widget_get_style_context(Edit_Information);
    // gtk_style_context_add_provider(Edit_Information_context, GTK_STYLE_PROVIDER(button_provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    GtkWidget *Apply_Loan = gtk_button_new_with_label("Apply For Loan");
    gtk_fixed_put(GTK_FIXED(fix), Apply_Loan, 25, 230);
    gtk_widget_set_size_request(Apply_Loan, 300, 20);
    gtk_widget_set_name(Apply_Loan, "login_button");
    GtkStyleContext *Apply_Loan_context = gtk_widget_get_style_context(Apply_Loan);
    gtk_style_context_add_provider(Apply_Loan_context, GTK_STYLE_PROVIDER(button_provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    GtkWidget *Check_Loan = gtk_button_new_with_label("Check Loan Status");
    gtk_fixed_put(GTK_FIXED(fix), Check_Loan, 25, 280);
    gtk_widget_set_size_request(Check_Loan, 300, 20);
    gtk_widget_set_name(Check_Loan, "login_button");
    GtkStyleContext *Check_Loan_context = gtk_widget_get_style_context(Check_Loan);
    gtk_style_context_add_provider(Check_Loan_context, GTK_STYLE_PROVIDER(button_provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    // GtkWidget *Payments_history = gtk_button_new_with_label("Payments History");
    // gtk_fixed_put(GTK_FIXED(fix), Payments_history, 25, 330);
    // gtk_widget_set_size_request(Payments_history, 300, 20);
    // gtk_widget_set_name(Payments_history, "login_button");
    // GtkStyleContext *Payments_history_context = gtk_widget_get_style_context(Payments_history);
    // gtk_style_context_add_provider(Payments_history_context, GTK_STYLE_PROVIDER(button_provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    // GtkWidget *Loan_History = gtk_button_new_with_label("Loan Application History");
    // gtk_fixed_put(GTK_FIXED(fix), Loan_History, 25, 380);
    // gtk_widget_set_size_request(Loan_History, 300, 20);
    // gtk_widget_set_name(Loan_History, "login_button");
    // GtkStyleContext *Loan_History_context = gtk_widget_get_style_context(Loan_History);
    // gtk_style_context_add_provider(Loan_History_context, GTK_STYLE_PROVIDER(button_provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    GtkWidget *Log_Out = gtk_button_new_with_label("Log Out");
    gtk_fixed_put(GTK_FIXED(fix), Log_Out, 25, 430);
    gtk_widget_set_size_request(Log_Out, 300, 20);
    gtk_widget_set_name(Log_Out, "back_button");
    GtkStyleContext *Log_Out_context = gtk_widget_get_style_context(Log_Out);
    gtk_style_context_add_provider(Log_Out_context, GTK_STYLE_PROVIDER(back_button_provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    g_object_set_data(G_OBJECT(Home_Page), "Page", GINT_TO_POINTER(1));
    g_signal_connect(Apply_Loan, "clicked", G_CALLBACK(Apply_Loan_UI), Home_Page);
    g_signal_connect(Check_Loan, "clicked", G_CALLBACK(Loan_Status_UI), Home_Page);
    g_signal_connect(Log_Out, "clicked", G_CALLBACK(First_Page_UI), Home_Page);




    gtk_window_set_child(GTK_WINDOW(Home_Page), fix);

    gtk_widget_set_visible(Home_Page, TRUE);
    gtk_widget_set_visible(main_window, FALSE);
}