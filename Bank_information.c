//
// Created by PC on 23-Nov-24.
//

#include "Bank_information.h"
#include "Check_Bank_Information.h"
#include "Structures/Bank_User_info.h"
#include "Save_file.h"
#include "First_Page.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
#include <ctype.h>
#include "Send_Email.h"

extern User new_user;
Bank_User_info new_bank_user_info;
const char* get_dropdown_item(GtkDropDown *dropdown) {
    GtkStringObject *item = gtk_drop_down_get_selected_item(dropdown);
    const char *year_str = gtk_string_object_get_string(item);
    return year_str;
}

void Check_Bank_Information(GtkWidget *widget, gpointer user_data) {
    //===========================================================
    GtkWidget *Bank_Information = GTK_WIDGET(user_data);

    GtkWidget *Bank_Name_Entry = GTK_WIDGET(g_object_get_data(G_OBJECT(Bank_Information), "Bank_Name_Entry"));
    const gchar *Bank_Name_text = get_dropdown_item(GTK_DROP_DOWN(Bank_Name_Entry));
    GtkWidget *Bank_Name_Button = g_object_get_data(G_OBJECT(Bank_Information), "Bank_Name_Button");

    GtkWidget *Account_Type_Entry = GTK_WIDGET(g_object_get_data(G_OBJECT(Bank_Information), "Account_Type_Entry"));
    const gchar *Account_Type_text = get_dropdown_item(GTK_DROP_DOWN(Account_Type_Entry));
    GtkWidget *Account_Type_Button = g_object_get_data(G_OBJECT(Bank_Information), "Account_Type_Button");

    GtkWidget *Account_Holder_Name_Entry = GTK_WIDGET(g_object_get_data(G_OBJECT(Bank_Information), "Account_Holder_Name_Entry"));
    GtkEntryBuffer *Account_Holder_Name_Buffer = gtk_entry_get_buffer(GTK_ENTRY(Account_Holder_Name_Entry));
    const gchar *Account_Holder_Name_text = gtk_entry_buffer_get_text(Account_Holder_Name_Buffer);
    GtkWidget *Account_Holder_Name_Button = g_object_get_data(G_OBJECT(Bank_Information), "Account_Holder_Name_Button");

    GtkWidget *Account_Number_Entry = GTK_WIDGET(g_object_get_data(G_OBJECT(Bank_Information), "Account_Number_Entry"));
    GtkEntryBuffer *Account_Number_Buffer = gtk_entry_get_buffer(GTK_ENTRY(Account_Number_Entry));
    const gchar *Account_Number_text = gtk_entry_buffer_get_text(Account_Number_Buffer);
    GtkWidget *Account_Number_Button = g_object_get_data(G_OBJECT(Bank_Information), "Account_Number_Button");

    // Check if any field is empty
    if (is_empty(Bank_Name_text) || is_empty(Account_Type_text) || is_empty(Account_Holder_Name_text) || is_empty(Account_Number_text)) {
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(Bank_Information), GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "All Fields are Required");
        g_signal_connect(dialog, "response", G_CALLBACK(gtk_window_close), NULL);
        gtk_widget_show(dialog);
        return;
    }

    // Validate the bank information
    gboolean valid_account_holder = is_Account_Holder_Name_Valid(Account_Holder_Name_text);
    gboolean valid_account_number = is_Account_Number_Valid(Account_Number_text, Bank_Name_text);



    gtk_widget_set_visible(Account_Holder_Name_Button, !valid_account_holder);
    if (!valid_account_holder) {
        gtk_widget_set_tooltip_text(Account_Holder_Name_Button, "Invalid Account Holder Name It Should Contain Only Letters");
    }

    gtk_widget_set_visible(Account_Number_Button, !valid_account_number);
    if (!valid_account_number) {
        gtk_widget_set_tooltip_text(Account_Number_Button, "Invalid Account Number");
    }

    // If everything is valid, save the bank information and proceed
    if (valid_account_holder && valid_account_number) {
        gtk_widget_set_visible(Bank_Name_Button, FALSE);
        gtk_widget_set_visible(Account_Holder_Name_Button, FALSE);
        gtk_widget_set_visible(Account_Number_Button, FALSE);

        new_bank_user_info.bank_id = new_user.user_id;
        new_bank_user_info.user_id = new_user.user_id;
        strcpy(new_bank_user_info.bank_name, Bank_Name_text);
        strcpy(new_bank_user_info.account_type, Account_Type_text);
        strcpy(new_bank_user_info.account_holder_name, Account_Holder_Name_text);
        strcpy(new_bank_user_info.account_number, Account_Number_text);
        time(&new_bank_user_info.created_at);
        time(&new_bank_user_info.updated_at);

        if (Save_Signup_Information(new_user, new_bank_user_info)) {
            FILE *cin_file = fopen("..\\DataBase\\CIN.txt", "a");
        if (cin_file != NULL) {
            fprintf(cin_file, "%s\n", new_user.cin);
            fclose(cin_file);
        }

        FILE *email_file = fopen("..\\DataBase\\Email.txt", "a");
        if (email_file != NULL) {
            fprintf(email_file, "%s\n", new_user.email);
            fclose(email_file);
        }
            // Send email to the user after successful signup
            char subject[256] = "Vos informations de connexion à NULOAN";
            char message[1024];

            // Update the subject line to ensure consistency
            snprintf(subject, sizeof(subject), "Bienvenue chez NULOAN!");

            // Create the message body with a clear and professional tone
            snprintf(message, sizeof(message),
                     "Bonjour %s %s,\n\n"
                     "Bienvenue dans l’univers de NULOAN! Nous sommes ravis de vous accueillir parmi nos utilisateurs et nous nous engageons à vous accompagner dans la gestion de vos crédits bancaires.\n\n"
                     "Voici vos informations de connexion pour accéder à votre compte :\n\n"
                     "   - Identifiant utilisateur : %d\n\n"
                     "Pour commencer, nous vous invitons à télécharger l’application NULOAN depuis l’App Store ou Google Play, puis à vous connecter en utilisant votre identifiant utilisateur.\n\n"
                     "🔒 **Sécurité de votre compte**\n"
                     "Pour garantir la sécurité de vos données, nous vous recommandons de ne **pas partager vos informations de connexion** avec quiconque.\n\n"
                     "📧 **Besoin d’assistance?**\n"
                     "Si vous avez des questions ou besoin d’aide, notre équipe est à votre disposition. Vous pouvez nous contacter à tout moment à l’adresse suivante : nuloan.project@gmail.com.\n\n"
                     "Nous sommes impatients de vous accompagner dans la gestion de vos projets financiers avec NULOAN!\n\n"
                     "Cordialement,\n"
                     "L’équipe NULOAN",
                     new_user.first_name, new_user.last_name, new_user.user_id);



            if (send_email(new_user.email, subject, message) != 0) {
                fprintf(stderr, "Error: Failed to send the welcome email.\n");
            }

                    First_Page_UI(NULL, Bank_Information);
                }
            }
        }
void Bank_Information_open(gpointer user_data) {

    GtkWidget *main_window = GTK_WIDGET(user_data);
    // Create the Bank Information window
    GtkWidget *Bank_Information = gtk_window_new();
gtk_window_set_title(GTK_WINDOW(Bank_Information), "Bank Information");
    gtk_window_set_default_size(GTK_WINDOW(Bank_Information), 350, 600);
    gtk_window_set_resizable(GTK_WINDOW(Bank_Information), FALSE);

    // Create a fixed layout for the Bank Information window
    GtkWidget *Bank_Information_Fix = gtk_fixed_new();
    gtk_window_set_child(GTK_WINDOW(Bank_Information), Bank_Information_Fix);

    GtkCssProvider *background_provider = gtk_css_provider_new();
    gtk_css_provider_load_from_string(background_provider,
    "window { "
    "   background: linear-gradient(to right, #002A70, #227593); "
    "}"
    );

    GtkCssProvider *entry_provider = gtk_css_provider_new();
    gtk_css_provider_load_from_string(entry_provider,
    "entry { "
    "  background-color: rgba(255, 255, 255, 0); "  // Semi-transparent white
    "  border: 1px solid #ffffff; "  // Light border for better visibility
    "  border-radius: 5px; "  // Adjust corner roundness
    "  padding: 5px; "  // Reduce padding
    "  min-height: 20px; "  // Enforce smaller height
    "  min-width: 200px; "  // Enforce width
    "  font-size: 15px; "
    "  color: #ffffff;  "
    "} "
    );





    gtk_style_context_add_provider_for_display(gdk_display_get_default(),
    GTK_STYLE_PROVIDER(background_provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    // Bank Name entry
    const gchar *valid_bank_names[] = {
        "Attijariwafa Bank", "BMCE", "Bank of Africa", "Banque Populaire",
        "BPMC", "CIH", "Crédit du Maroc", "Société Générale",
        "BMCI", "Poste Maroc", "Barid Bank",NULL
    };
    GtkStringList *bank_names_model = gtk_string_list_new(valid_bank_names);
    GtkWidget *bank_name_dropdown = gtk_drop_down_new(G_LIST_MODEL(bank_names_model), NULL);

    // Position the dropdown in the same way as the original GtkEntry
    gtk_fixed_put(GTK_FIXED(Bank_Information_Fix), bank_name_dropdown, 45, 200);
    gtk_widget_set_size_request(bank_name_dropdown, 250, 25);

    //Bank Name image
    GtkWidget *Bank_Name_Button = gtk_button_new();
    GtkWidget *Bank_Name_image = gtk_image_new_from_file("..\\images\\info_red.png");
    gtk_button_set_child(GTK_BUTTON(Bank_Name_Button), Bank_Name_image);
    gtk_fixed_put(GTK_FIXED(Bank_Information_Fix), Bank_Name_Button, 300, 200);
    gtk_widget_set_size_request(Bank_Name_Button, 20, 20);
    gtk_widget_set_visible(Bank_Name_Button, FALSE);
    gtk_widget_set_sensitive(Bank_Name_Button, FALSE);


    // Account Type entry
    // Account Type entry (New Code)
    const gchar *bank_types[] = {
        "Savings", "Checking", "Business", "Joint", "Current", NULL
    };
    GtkStringList *bank_types_model = gtk_string_list_new(bank_types);
    GtkWidget *bank_type_dropdown = gtk_drop_down_new(G_LIST_MODEL(bank_types_model), NULL);

    // Position the dropdown in the same way as the Account Type entry
    gtk_fixed_put(GTK_FIXED(Bank_Information_Fix), bank_type_dropdown, 45, 240);
    gtk_widget_set_size_request(bank_type_dropdown, 250, 25);
    //Bank Name image
    GtkWidget *Account_Type_Button = gtk_button_new();
    GtkWidget *Account_Type_image = gtk_image_new_from_file("..\\images\\info_red.png");
    gtk_button_set_child(GTK_BUTTON(Account_Type_Button), Account_Type_image);
    gtk_fixed_put(GTK_FIXED(Bank_Information_Fix), Account_Type_Button, 300, 240);
    gtk_widget_set_size_request(Account_Type_Button, 20, 20);
    gtk_widget_set_visible(Account_Type_Button, FALSE);
    gtk_widget_set_sensitive(Account_Type_Button, FALSE);

    // Account Holder Name entry
    GtkWidget *Account_Holder_Name_Entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(Account_Holder_Name_Entry), "Account Holder Name");
    gtk_fixed_put(GTK_FIXED(Bank_Information_Fix), Account_Holder_Name_Entry, 45, 280);
    gtk_widget_set_size_request(Account_Holder_Name_Entry, 250, 25);
    GtkStyleContext *Account_Holder_Name_context = gtk_widget_get_style_context(Account_Holder_Name_Entry);
    gtk_style_context_add_provider(Account_Holder_Name_context, GTK_STYLE_PROVIDER(entry_provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    //Bank Name image
    GtkWidget *Account_Holder_Name_Button = gtk_button_new();
    GtkWidget *Account_Holder_Name_image = gtk_image_new_from_file("..\\images\\info_red.png");
    gtk_button_set_child(GTK_BUTTON(Account_Holder_Name_Button), Account_Holder_Name_image);
    gtk_fixed_put(GTK_FIXED(Bank_Information_Fix), Account_Holder_Name_Button, 300, 280);
    gtk_widget_set_size_request(Account_Holder_Name_Button, 20, 20);
    gtk_widget_set_visible(Account_Holder_Name_Button, FALSE);
    gtk_widget_set_sensitive(Account_Holder_Name_Button, FALSE);

    // Account Number entry
    GtkWidget *Account_Number_Entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(Account_Number_Entry), "Account Number");
    gtk_fixed_put(GTK_FIXED(Bank_Information_Fix), Account_Number_Entry, 45, 320);
    gtk_widget_set_size_request(Account_Number_Entry, 250, 25);
    GtkStyleContext *Account_Number_context = gtk_widget_get_style_context(Account_Number_Entry);
    gtk_style_context_add_provider(Account_Number_context, GTK_STYLE_PROVIDER(entry_provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    //Bank Name image
    GtkWidget *Account_Number_Button = gtk_button_new();
    GtkWidget *Account_Number_image = gtk_image_new_from_file("..\\images\\info_red.png");
    gtk_button_set_child(GTK_BUTTON(Account_Number_Button), Account_Number_image);
    gtk_fixed_put(GTK_FIXED(Bank_Information_Fix), Account_Number_Button, 300, 320);
    gtk_widget_set_size_request(Account_Number_Button, 20, 20);
    gtk_widget_set_visible(Account_Number_Button, FALSE);
    gtk_widget_set_sensitive(Account_Number_Button, FALSE);

    g_object_set_data(G_OBJECT(Bank_Information), "Bank_Information_Fix", Bank_Information_Fix);
    g_object_set_data(G_OBJECT(Bank_Information), "Bank_Name_Entry", bank_name_dropdown);
    g_object_set_data(G_OBJECT(Bank_Information), "Bank_Name_Button", Bank_Name_Button);
    g_object_set_data(G_OBJECT(Bank_Information), "Account_Type_Entry", bank_type_dropdown);
    g_object_set_data(G_OBJECT(Bank_Information), "Account_Type_Button", Account_Type_Button);
    g_object_set_data(G_OBJECT(Bank_Information), "Account_Holder_Name_Entry", Account_Holder_Name_Entry);
    g_object_set_data(G_OBJECT(Bank_Information), "Account_Holder_Name_Button", Account_Holder_Name_Button);
    g_object_set_data(G_OBJECT(Bank_Information), "Account_Number_Entry", Account_Number_Entry);
    g_object_set_data(G_OBJECT(Bank_Information), "Account_Number_Button", Account_Number_Button);

    // Create Submit Button
    GtkWidget *Submit = gtk_button_new_with_label("Submit");
    gtk_fixed_put(GTK_FIXED(Bank_Information_Fix), Submit, 45, 360);
    gtk_widget_set_size_request(Submit, 250, 20);
    g_signal_connect(Submit, "clicked", G_CALLBACK(Check_Bank_Information), Bank_Information);

    gtk_window_set_child(GTK_WINDOW(Bank_Information), Bank_Information_Fix);


    // Show all widgets
    gtk_window_present(GTK_WINDOW(Bank_Information));
    gtk_widget_hide(main_window);
}
