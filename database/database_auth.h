#ifndef DATABASE_AUTH_H
#define DATABASE_AUTH_H
#include "database_connect.h"
#include "database_query.h"
#include <openssl/sha.h>


typedef struct{
    int u_id;
    char* salt;
    char* hashed_password;
}auth_data;


char* hash_password(const char* password, const char* salt);
int verify_password(const char* username,const char* entered_password);

auth_data* get_auth_data(const char* username);
#endif

