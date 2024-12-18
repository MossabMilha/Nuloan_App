

#ifndef CHECK_USER_INFORMATION_H
#define CHECK_USER_INFORMATION_H
void check_first_name(const char *first_name, GtkWidget *first_name_button);
void check_last_name(const char *last_name, GtkWidget *last_name_button);
void check_CIN(const char *cin, GtkWidget *cin_button);
void check_email(const char *email,GtkWidget *gmail_button);
void check_birthday(const char *birthday, GtkWidget *birthday_button);
char* check_password(const gchar *password, const gchar *confirm_password, const gchar *first_name, const gchar *last_name, GtkWidget *password_button);

#endif //CHECK_USER_INFORMATION_H
