#ifndef DATABASE_CONNECT_H
#define DATABASE_CONNECT_H
#include <libpq-fe.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <erproc.h>

#define DB_NAME  "selsckogram"
#define DB_HOST  "localhost"
#define DB_USER  "andrey"
#define DB_PORT  "5432"
#define DB_PASSWORD "525252"

extern PGconn* _connect;

int db_connect();
void db_close();
int db_connection_status();
#endif // DATABASE_CONNECT_H
