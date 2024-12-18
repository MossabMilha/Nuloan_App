
#ifndef CHECK_BANK_INFORMATION_H
#define CHECK_BANK_INFORMATION_H
#include <gtk/gtk.h>

bool is_empty(const gchar *elm_text);
bool is_Account_Holder_Name_Valid(const gchar *text);
bool is_Account_Number_Valid(const gchar *text, const gchar *bank_name);
#endif
