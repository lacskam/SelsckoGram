#include "database.h"


PGconn* _connect=NULL;

int db_connect()
{
    char connect_params[256];
    snprintf(connect_params,sizeof(connect_params),
            "host=%s dbname=%s user=%s password=%s port=%s",
             DB_HOST,DB_NAME,DB_USER,DB_PASSWORD,DB_PORT);
    _connect=PQconnectdb(connect_params);
    if(PQstatus(_connect)!=CONNECTION_OK) {
        printf("[ERROR] - Connection failed: %s - [db_connect]\n",PQerrorMessage(_connect));
        PQfinish(_connect);
        return -1;
    }
    return 0;
}

void db_close()
{
    if(_connect && PQstatus(_connect)==CONNECTION_OK) {
        PQfinish(_connect);
        _connect=NULL;
        puts("[LOG] - Database connection closed - [db_close]");
    }
}


int db_connection_status()
{
    if(!_connect) {
        puts("[ERROR] - _connect is not init - [db_connection_status]");
        return -1;
    }
    if(PQstatus(_connect)!=CONNECTION_OK) {
        puts("[ERROR] - the connection to the database was lost - [db_connection_status]");
        db_close();
        return db_connect();
    }
    return 0;
}
unsigned int generate_salt()
{
    time_t current_time;
    current_time=time(NULL);
    //printf("%u",(unsigned int)current_time);
    return (unsigned int)current_time;
}

