//
// Created by PC on 23-Nov-24.
//

#include "Check_Bank_Information.h"

#include <string.h>
#include <gtk/gtk.h>
#include <ctype.h>
bool is_empty(const gchar *elm_text) {
    if(strlen(elm_text) == 0){
        return TRUE;
    }
    return FALSE;
}

bool is_Account_Holder_Name_Valid(const gchar *text) {
    int i = 0;
    while (text[i] != '\0') {
        if (!(isalpha(text[i]) || text[i] == ' ')) {
            return FALSE;
        }
        i++;
    }
    return  TRUE;
}

bool is_Account_Number_Valid(const gchar *text, const gchar *bank_name) {
    typedef struct {
        const gchar *bank_name;
        int account_length;
    } BankInfo;

    BankInfo banks[] = {
        {"Attijariwafa Bank", 24},
        {"BMCE", 24},
        {"Bank of Africa", 24},
        {"Banque Populaire", 24},
        {"BPMC", 24},
        {"CIH", 24},
        {"Crédit du Maroc", 24},
        {"Société Générale", 24},
        {"BMCI", 24},
        {"Poste Maroc", 20},
        {"Barid Bank", 20},
    };

    for (int i = 0; i < sizeof(banks) / sizeof(banks[0]); i++) {
        if (g_ascii_strcasecmp(bank_name, banks[i].bank_name) == 0) {
            if (strlen(text) != banks[i].account_length) {
                return FALSE;
            }
            for (int j = 0; j < banks[i].account_length; j++) {
                if (!isdigit(text[j])) {
                    return FALSE;
                }
            }
            return TRUE;
        }
    }
    return FALSE;
}