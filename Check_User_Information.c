#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
#include <regex.h>
#include <time.h>
#include <gcrypt.h>
extern char global_first_name[50];
extern char global_last_name[50];
extern char global_cin[10];
extern char global_email[50];
extern char global_birthday[11];
extern char global_password[50];
#define MIN_PASSWORD_LENGTH 8
#define SPECIAL_CHARACTERS "!@#$%^&*()-_=+[]{}|;:'\",.<>?/\\"

void check_first_name(const char *first_name, GtkWidget *first_name_button) {
    const char *temp = first_name;
    while (*temp) {
        if (isdigit(*temp)) {
            gtk_widget_set_visible(first_name_button, TRUE);
            return;
        }
        temp++;
    }
    strncpy(global_first_name, first_name, sizeof(global_first_name) - 1);
    global_first_name[sizeof(global_first_name) - 1] = '\0';
    g_print("first : %s\n", global_first_name);
    gtk_widget_set_visible(first_name_button, FALSE);
}

void check_last_name(const char *last_name, GtkWidget *last_name_button) {
    const char *temp = last_name;
    while (*temp) {
        if (isdigit(*temp)) {
            gtk_widget_set_visible(last_name_button, TRUE);
            return;
        }
        temp++;
    }
    strncpy(global_last_name, last_name, sizeof(global_last_name) - 1);
    global_last_name[sizeof(global_last_name) - 1] = '\0';
    gtk_widget_set_visible(last_name_button, FALSE);
}

void check_CIN(const char *cin, GtkWidget *cin_button) {
    regex_t regex;
    int reti;

    // Compile the regular expression
    reti = regcomp(&regex, "^[A-Za-z]{1,2}[0-9]{1,6}$", REG_EXTENDED);
    if (reti) {
        fprintf(stderr, "Could not compile regex\n");
        return;
    }

    // Execute the regular expression
    reti = regexec(&regex, cin, 0, NULL, 0);
    if (!reti) {
        // Check if the CIN already exists in CIN.txt
        FILE *file = fopen("..\\DataBase\\CIN.txt", "r");
        if (file != NULL) {
            char line[100];
            while (fgets(line, sizeof(line), file) != NULL) {
                line[strcspn(line, "\n")] = '\0';  // Remove newline character
                if (strcmp(line, cin) == 0) {
                    gtk_widget_set_visible(cin_button, TRUE);
                    gtk_widget_set_tooltip_text(cin_button, "CIN is already used");
                    fclose(file);
                    regfree(&regex);
                    return;
                }
            }
            fclose(file);
        }

        // CIN is valid and not in use
        strncpy(global_cin, cin, sizeof(global_cin) - 1);
        global_cin[sizeof(global_cin) - 1] = '\0';
        gtk_widget_set_visible(cin_button, FALSE);
    } else if (reti == REG_NOMATCH) {
        // CIN is invalid
        gtk_widget_set_visible(cin_button, TRUE);
        gtk_widget_set_tooltip_text(cin_button, "Enter A Valid CIN Number");
    } else {
        char msgbuf[100];
        regerror(reti, &regex, msgbuf, sizeof(msgbuf));
        fprintf(stderr, "Regex match failed: %s\n", msgbuf);
    }

    // Free the compiled regular expression
    regfree(&regex);
}

void check_email(const char *email, GtkWidget *gmail_button) {
    regex_t regex;
    int reti;

    // Define the regular expression for a valid Gmail email format
    const char *pattern = "^[A-Za-z0-9._%+-]+@gmail\\.com$";

    // Compile the regular expression
    reti = regcomp(&regex, pattern, REG_EXTENDED);
    if (reti) {
        fprintf(stderr, "Could not compile regex\n");
    }

    // Execute the regular expression
    reti = regexec(&regex, email, 0, NULL, 0);
    regfree(&regex);

    if (!reti) {
        // Check if the email already exists in Email.txt
        FILE *file = fopen("..\\DataBase\\Email.txt", "r");
        if (file != NULL) {
            char line[100];
            while (fgets(line, sizeof(line), file) != NULL) {
                line[strcspn(line, "\n")] = '\0';  // Remove newline character
                if (strcmp(line, email) == 0) {
                    gtk_widget_set_visible(gmail_button, TRUE);
                    gtk_widget_set_tooltip_text(gmail_button, "Email is already used");
                    fclose(file);
                    return;
                }
            }
            fclose(file);
        }

        gtk_widget_set_visible(gmail_button, FALSE); // Email format is valid
        strncpy(global_email, email, sizeof(global_email) - 1);
        global_email[sizeof(global_email) - 1] = '\0';
    } else if (reti == REG_NOMATCH) {
        gtk_widget_set_visible(gmail_button, TRUE); // Email format is invalid
        gtk_widget_set_tooltip_text(gmail_button, "Enter A Valid Email");
    } else {
        char msgbuf[100];
        regerror(reti, &regex, msgbuf, sizeof(msgbuf));
        fprintf(stderr, "Regex match failed: %s\n", msgbuf);
    }
}

void check_birthday(const char *birthday, GtkWidget *birthday_button) {
    regex_t regex;
    int reti;

    // Define the regular expression for a valid birthday format
    const char *pattern = "^[0-3][0-9]/[0-1][0-9]/[0-9]{4}$";

    // Compile the regular expression
    reti = regcomp(&regex, pattern, REG_EXTENDED);
    if (reti) {
        fprintf(stderr, "Could not compile regex\n");
        return;
    }

    // Execute the regular expression
    reti = regexec(&regex, birthday, 0, NULL, 0);
    if (reti == REG_NOMATCH) {
        gtk_widget_set_visible(birthday_button, TRUE); // Invalid format
        regfree(&regex);
        return;
    }

    // Parse the date
    int day, month, year;
    sscanf(birthday, "%2d/%2d/%4d", &day, &month, &year);

    // Get the current date
    time_t t = time(NULL);
    struct tm *current_time = localtime(&t);

    // Calculate the age
    int age = current_time->tm_year + 1900 - year;
    if (current_time->tm_mon + 1 < month || (current_time->tm_mon + 1 == month && current_time->tm_mday < day)) {
        age--;
    }

    // Check if the user is more than 18 years old
    if (age >= 18) {
        gtk_widget_set_visible(birthday_button, FALSE); // Valid birthday
        strncpy(global_birthday, birthday, sizeof(global_birthday) - 1);
        global_birthday[sizeof(global_birthday) - 1] = '\0';
    } else {
        gtk_widget_set_visible(birthday_button, TRUE); // Invalid birthday
    }

    // Free the compiled regular expression
    regfree(&regex);
}

char* check_password(const gchar *password, const gchar *confirm_password, const gchar *first_name, const gchar *last_name, GtkWidget *password_button) {
    printf("%s==%s\n", password, confirm_password);

    static char message[1024] = "";
    message[0] = '\0';  // Clear previous message

    // Check if password and confirmation password are identical
    if (strcmp(password, confirm_password) != 0) {
        strcat(message, "The Password and The Confirmation should be the same.\n");
    }

    // Check if password is in the most used password list
    char content[1000];
    FILE *file = fopen("..\\Useful Files\\Most_Used_Password.txt", "r");
    if (file != NULL) {
        while (fgets(content, sizeof(content), file) != NULL) {
            content[strcspn(content, "\n")] = '\0';  // Remove newline character
            if (strcmp(content, password) == 0) {
                strcat(message, "Weak password: easy to guess.\n");
                break;
            }
        }
        fclose(file);
    } else {
        strcat(message, "Could not open password file.\n");
    }

    // Check if password contains part of the user's name
    if (strstr(password, first_name) != NULL || strstr(password, last_name) != NULL) {
        strcat(message, "Your password contains part of your name.\n");
    }

    // Check password length
    if (strlen(password) < MIN_PASSWORD_LENGTH) {
        strcat(message, "Password must be at least 8 characters long.\n");
    }

    // Check for uppercase, lowercase, digit, and special character
    int has_upper = 0, has_lower = 0, has_digit = 0, has_special = 0;
    for (int i = 0; i < strlen(password); i++) {
        if (isupper(password[i])) has_upper = 1;
        if (islower(password[i])) has_lower = 1;
        if (isdigit(password[i])) has_digit = 1;
        if (strchr(SPECIAL_CHARACTERS, password[i])) has_special = 1;
    }

    if (!has_upper) strcat(message, "Add at least one uppercase letter.\n");
    if (!has_lower) strcat(message, "Add at least one lowercase letter.\n");
    if (!has_digit) strcat(message, "Add at least one digit.\n");
    if (!has_special) strcat(message, "Add at least one special character.\n");

    // Show or hide the button based on the validation results
    if (strlen(message) > 0) {
        gtk_widget_set_visible(password_button, TRUE);
        gtk_widget_set_tooltip_text(password_button, message);  // Set the tooltip text
        fprintf(stderr, "%s", message);  // Print the error message for debugging
    } else {
        gtk_widget_set_visible(password_button, FALSE);
        strncpy(global_password, password, sizeof(global_password) - 1);
        global_password[sizeof(global_password) - 1] = '\0';
    }

    return message;
}