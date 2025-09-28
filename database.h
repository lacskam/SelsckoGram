#ifndef DATABASE_H
#define DATABASE_H
#include <libpq-fe.h>
#include <time.h>
#include <stdio.h>

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


#endif // DATABASE_H
