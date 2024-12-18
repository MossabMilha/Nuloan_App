//
// Created by PC on 23-Nov-24.
//

#include "Save_file.h"

    bool Save_Signup_Information(User user, Bank_User_info bank_user_info) {
    char user_path[100], bank_user_info_path[100];
        printf("debug 3 : %d\n", user.user_id);
    snprintf(user_path, sizeof(user_path), "..\\DataBase\\Users\\user_%d.bin", user.user_id);
    snprintf(bank_user_info_path, sizeof(bank_user_info_path), "..\\DataBase\\Users_Bank_Information\\user_%d.bin", user.user_id);

    FILE *file = fopen(user_path, "wb");
    if (file == NULL) {
        fprintf(stderr, "Could not open file for writing\n");
        return false;
    }
    fwrite(&user, sizeof(User), 1, file);
    fclose(file);

    file = fopen(bank_user_info_path, "wb");
    if (file == NULL) {
        fprintf(stderr, "Could not open file for writing\n");
        return false;
    }
    fwrite(&bank_user_info, sizeof(Bank_User_info), 1, file);
    fclose(file);
    file = fopen("..\\DataBase\\Number_Of_Users.txt", "wb");
    if (file == NULL) {
        fprintf(stderr, "Could not open file for writing\n");
        return false;
    }
    int number_of_users = user.user_id;
        printf("debug 4 :%d\n", number_of_users);
    fprintf(file, "%d", number_of_users);
    fclose(file);

    return true;
}