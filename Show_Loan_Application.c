//
// Created by PC on 07-Dec-24.
//

#include "Show_Loan_Application.h"
//
// Created by PC on 07-Dec-24.
//

#include "Show_Loan_Application.h"
#include "Show_Loan_Application.h"
#include <gtk/gtk.h>
#include <dirent.h>
#include <sys/stat.h>
#include "Structures/Application.h"  // Include the struct for Loan_Apply
#include "Structures/User.h"
#include "Structures/Loans_Types.h"
#include "Approve_Reject_Application.h"
#include "Super_Admin.h"

int count_files_in_directory_1(const char *directory_path) {
    int file_count = 0;
    struct dirent *entry;
    DIR *dp = opendir(directory_path);

    if (dp == NULL) {
        return 0;
    }

    while ((entry = readdir(dp))) {
        struct stat path_stat;
        char full_path[1024];
        snprintf(full_path, sizeof(full_path), "%s/%s", directory_path, entry->d_name);
        stat(full_path, &path_stat);
        if (S_ISREG(path_stat.st_mode)) {
            file_count++;
        }
    }

    closedir(dp);
    return file_count;
}

// Function to read the loan application data from the binary file
Application Read_Loan_Application(int line_number) {
    FILE *id_file = fopen("..\\DataBase\\Id_Of_applications.txt", "r");
    if (id_file == NULL) {
        perror("Failed to open ID file");
        return (Application){0}; // Return an empty Loan_Apply struct
    }

    int id;
    int current_line = 0;
    while (fscanf(id_file, "%d", &id) != EOF) {
        if (current_line == line_number - 1) {
            break;
        }
        current_line++;
    }
    fclose(id_file);

    if (current_line != line_number - 1) {
        fprintf(stderr, "Line number %d not found in ID file\n", line_number);
        return (Application){0}; // Return an empty Loan_Apply struct
    }

    char bin_file_path[256];

    snprintf(bin_file_path, sizeof(bin_file_path), "C:\\Users\\PC\\CLionProjects\\nuloan_project\\DataBase\\Application_Not_Checked\\application_%d.bin", id);
    printf("%s\n", bin_file_path);
    FILE *bin_file = fopen(bin_file_path, "rb");
    if (bin_file == NULL) {
        perror("Failed to open binary file");
        return (Application){0}; // Return an empty Loan_Apply struct
    }


    Application loan_apply;
    fread(&loan_apply, sizeof(loan_apply), 1, bin_file);
    fclose(bin_file);

    return loan_apply;
}

// Function to read the user data from the binary file
User Read_User(int id) {
    char bin_file_path[256];
    snprintf(bin_file_path, sizeof(bin_file_path), "..\\DataBase\\User\\user_%d.bin", id);
    FILE *bin_file = fopen(bin_file_path, "rb");
    if (bin_file == NULL) {
        perror("Failed to open binary file");
        return (User){0}; // Return an empty User struct
    }
    User user;
    fread(&user, sizeof(user), 1, bin_file);
    fclose(bin_file);
    return user;
}

loan_type Read_Loan(int id) {
    char bin_file_path[256];
    snprintf(bin_file_path, sizeof(bin_file_path), "..\\DataBase\\LOANS\\loan_%d.bin", id);
    FILE *bin_file = fopen(bin_file_path, "rb");
    if (bin_file == NULL) {
        perror("Failed to open binary file");
        return (loan_type){0}; // Return an empty User struct
    }
    loan_type loan;
    fread(&loan, sizeof(loan), 1, bin_file);
    fclose(bin_file);
    return loan;
}
void Prev_Page_1(GtkWidget *widget, gpointer data) {
    int page = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(data), "Page"));
    if (page > 1) {
        page--;
        g_object_set_data(G_OBJECT(data), "Page", GINT_TO_POINTER(page));
        Check_Applications(widget, data);
    }
}
void Next_Page_1(GtkWidget *widget, gpointer data) {
    int page = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(data), "Page"));
    page++;
    g_object_set_data(G_OBJECT(data), "Page", GINT_TO_POINTER(page));
    Check_Applications(widget, data);
}


void Create_loan_application_Frame(GtkWidget *window,GtkWidget *parent, const Application loan, const int x, const int y) {
    loan_type loan_type = Read_Loan(loan.loan_id);

    GtkCssProvider *label_provider = gtk_css_provider_new();
    gtk_css_provider_load_from_string(label_provider,
        "label { "
        "   color: #FFFFFF; "
        "   font-family: Arial; "
        "   font-size: 17px; "
        "   padding: 5px;"
        "}");
    GtkWidget *frame = gtk_frame_new("");
    gtk_widget_set_size_request(frame, 250, 200);

    GtkWidget *loan_id_label = gtk_label_new("");
    char loan_id_text[100];
    sprintf(loan_id_text, "Loan Application ID: %d", loan.loan_application_id);
    gtk_label_set_text(GTK_LABEL(loan_id_label), loan_id_text);
    gtk_label_set_xalign(GTK_LABEL(loan_id_label), 0);
    GtkStyleContext *user_id_context = gtk_widget_get_style_context(loan_id_label);
    gtk_style_context_add_provider(user_id_context, GTK_STYLE_PROVIDER(label_provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    // Loan type label
    GtkWidget *loan_apply_id_label = gtk_label_new("");
    char loan_apply_id_text[100];
    sprintf(loan_apply_id_text, "Type Of Loan: %s", loan_type.loan_title);
    gtk_label_set_text(GTK_LABEL(loan_apply_id_label), loan_apply_id_text);
    gtk_label_set_xalign(GTK_LABEL(loan_apply_id_label), 0);
    GtkStyleContext *loan_type_context = gtk_widget_get_style_context(loan_apply_id_label);
    gtk_style_context_add_provider(loan_type_context, GTK_STYLE_PROVIDER(label_provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    // Amount needed label
    GtkWidget *amount_needed_label = gtk_label_new("");
    char amount_text[100];
    sprintf(amount_text, "Amount Needed: %.2f", loan.amount_requested);
    gtk_label_set_text(GTK_LABEL(amount_needed_label), amount_text);
    gtk_label_set_xalign(GTK_LABEL(amount_needed_label), 0);
    GtkStyleContext *amount_needed_context = gtk_widget_get_style_context(amount_needed_label);
    gtk_style_context_add_provider(amount_needed_context, GTK_STYLE_PROVIDER(label_provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    // Duration needed label
    GtkWidget *duration_needed_label = gtk_label_new("");
    char duration_text[100];
    sprintf(duration_text, "Duration: %d months", loan.loan_duration);
    gtk_label_set_text(GTK_LABEL(duration_needed_label), duration_text);
    gtk_label_set_xalign(GTK_LABEL(duration_needed_label), 0);
    GtkStyleContext *duration_needed_context = gtk_widget_get_style_context(duration_needed_label);
    gtk_style_context_add_provider(duration_needed_context, GTK_STYLE_PROVIDER(label_provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    GtkWidget *more_button = gtk_button_new_with_label("More");

    // Allocate and set up a gpointer array to pass multiple parameters
    gpointer *tab = g_malloc(sizeof(gpointer) * 2);
    tab[0] = GINT_TO_POINTER(loan.loan_application_id); // Store loan ID
    tab[1] = window; // Store parent (Apply_Loan_window in your example)

    // Connect the button signal and pass the array as user data
    g_signal_connect(more_button, "clicked", G_CALLBACK(Approve_Activate), tab);

    // Create a vertical box to hold all widgets
    GtkWidget *frame_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_box_append(GTK_BOX(frame_box), loan_id_label);
    gtk_box_append(GTK_BOX(frame_box), loan_apply_id_label);
    gtk_box_append(GTK_BOX(frame_box), amount_needed_label);
    gtk_box_append(GTK_BOX(frame_box), duration_needed_label);
    gtk_box_append(GTK_BOX(frame_box), more_button);

    gtk_frame_set_child(GTK_FRAME(frame), frame_box);
    gtk_fixed_put(GTK_FIXED(parent), frame, x, y);
}

void Check_Applications(GtkWidget *widget, gpointer data) {
    int page = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(data), "Page"));
    GtkWidget *Admin_Page_1 = GTK_WIDGET(data);
    GtkWidget *Admin_Page_Fix = g_object_get_data(G_OBJECT(Admin_Page_1), "Admin_Page_Fix");
    GtkWidget *Admin_OutPut_Fix = g_object_get_data(G_OBJECT(Admin_Page_1), "Admin_OutPut_Fix");

    GtkWidget *Exit = gtk_button_new_with_label("Exit");
    gtk_fixed_put(GTK_FIXED(Admin_Page_Fix), Exit, 75, 400);
    gtk_widget_set_size_request(Exit, 250, 20);
    gtk_widget_set_visible(Exit, TRUE);
    g_signal_connect(Exit, "clicked", G_CALLBACK(Admin_Page),  data);

    // Clear previous content
    GtkWidget *child = gtk_widget_get_first_child(GTK_WIDGET(Admin_OutPut_Fix));
    while (child != NULL) {
        GtkWidget *next_child = gtk_widget_get_next_sibling(child);
        gtk_widget_unparent(child);
        child = next_child;
    }

    // Create Page Title
    GtkWidget *Title = gtk_label_new("Loan Application Information");
    gtk_label_set_markup(GTK_LABEL(Title), "<span font='20' weight='bold' foreground='#000000'>Loan Application Information</span>");
    gtk_fixed_put(GTK_FIXED(Admin_OutPut_Fix), Title, 40, 50);

    // Create fixed layout for loan applications
    GtkWidget *grid = gtk_grid_new();
    gtk_fixed_put(GTK_FIXED(Admin_OutPut_Fix), grid, 10, 100);
    int max = 1;
    if (page * 10 > count_files_in_directory_1("..\\DataBase\\Application_Not_Checked")) {
        max = count_files_in_directory_1("..\\DataBase\\Application_Not_Checked") % 10;
    } else {
        max = 10;
    }

    for (int i = 0; i < max; i++) {
        Application loan = Read_Loan_Application(i + 1 + (page - 1) * 10);
        int x = (i % 5) * 290;
        int y = (i / 5) * 250 + 100;
        Create_loan_application_Frame(Admin_Page_1,Admin_OutPut_Fix, loan, x, y);
    }

    // Create navigation buttons
    GtkWidget *prev_button = gtk_button_new_with_label("Previous");
    gtk_fixed_put(GTK_FIXED(Admin_OutPut_Fix), prev_button, 0, 600);
    g_signal_connect(prev_button, "clicked", G_CALLBACK(Prev_Page_1), Admin_Page_1);

    GtkWidget *next_button = gtk_button_new_with_label("Next");
    gtk_fixed_put(GTK_FIXED(Admin_OutPut_Fix), next_button, 1300, 600);
    g_signal_connect(next_button, "clicked", G_CALLBACK(Next_Page_1), Admin_Page_1);

    // Show or hide buttons based on the page number
    if (page == 1) {
        gtk_widget_set_visible(prev_button, FALSE);
    } else {
        gtk_widget_set_visible(prev_button, TRUE);
    }
    int total_files = count_files_in_directory_1("..\\DataBase\\Application_Not_Checked");
    int total_pages = (total_files + 9) / 10; // Calculate total pages needed

    if (page >= total_pages) {
        gtk_widget_set_visible(next_button, FALSE);
    } else {
        gtk_widget_set_visible(next_button, TRUE);
    }

    // Show all widgets
    gtk_widget_set_visible(Admin_OutPut_Fix, TRUE);
}
