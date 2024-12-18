// Created by: MMILHA
#include <string.h>
#include <gcrypt.h>
#include "hash_password.h"
#define key "1A5"
void hash_password(const char *password, unsigned char *output) {
    size_t key_len = strlen(key);
    gcry_md_hd_t hd;
    gcry_md_open(&hd, GCRY_MD_SHA256, GCRY_MD_FLAG_HMAC);
    gcry_md_setkey(hd, key, key_len);
    gcry_md_write(hd, password, strlen(password));
    memcpy(output, gcry_md_read(hd, 0), 32);
    gcry_md_close(hd);
}