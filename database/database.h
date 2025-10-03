#ifndef DATABASE_H
#define DATABASE_H
#include <libpq-fe.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <erproc.h>

#include <openssl/sha.h>

#define DB_NAME  "selsckogram"
#define DB_HOST  "localhost"
#define DB_USER  "andrey"
#define DB_PORT  "5432"
#define DB_PASSWORD "525252"


extern PGconn* _connect;

int db_connect();
void db_close();
int db_connection_status();
unsigned int generate_salt();
char* get_salt(const char* username);
char* hash_password(const char* password, const char* salt);
int verify_password(const char* username,const char* entered_password);
char* get_hashed_password(const char* username);
char* base_query(const char* query,int params_number, const char** params);
#endif // DATABASE_H
