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
        return -1;
    }
    return 0;
}
unsigned int generate_salt()
{
    time_t current_time;
    current_time=time(NULL);
    return (unsigned int)current_time;
}
char* get_hashed_password(const char* username)
{
    const char* query="SELECT hashed_password FROM accounts WHERE login=$1";
    const char* params[1]={username};
    char* password=base_query(query,1,params);
    return password;
}
char* get_salt(const char* username)
{

    const char* query="SELECT salt FROM accounts WHERE login=$1";
    const char* params[1]={username};

    char* salt=base_query(query,1,params);
    return salt;
}

char* hash_password(const char* password, const char* salt)
{

    size_t len=strlen(password)+strlen(salt);

    char* full_password=malloc(len+1);

    snprintf(full_password,len+1,"%s%s",password,salt);

    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256((unsigned char*)full_password, strlen(full_password), hash);
    free(full_password);

    char* hex_hash = malloc(SHA256_DIGEST_LENGTH * 2 + 1);

    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        sprintf(hex_hash + (i * 2), "%02x", hash[i]);
    }

    hex_hash[SHA256_DIGEST_LENGTH * 2] = '\0';

    return hex_hash;
}

int verify_password(const char *username,const char* entered_password)
{
    char* salt=get_salt(username);
    if(!salt) {
        return -1;
    }

    char* hash_entered_password=hash_password(entered_password,salt);
    if(!hash_entered_password) {
        free(salt);
        return -1;
    }
    char* db_password=get_hashed_password(username);
    if(!db_password) {
        free(salt);
        free(hash_entered_password);
        return -1;
    }
    int result=-1;
    if(strcmp(hash_entered_password,db_password)==0) {
        printf("[LOG] - Password verification successful for user:%s - [verify_password]\n",username);//выводить u_id
        result=0;
    }
    else {
        puts("[ERROR] - Wrong password - [verify_password]");
        result=-1;
    }

    free(salt);
    free(hash_entered_password);
    free(db_password);

    return result;
}

char *base_query(const char *query, int params_number, const char **params)
{
    if(db_connection_status()==-1)
    {
        return NULL;
    }
    char* result_str=NULL;

    PGresult *exec=PQexecParams(_connect,query,params_number,NULL,params,NULL,NULL,0);

    if(PQresultStatus(exec)==PGRES_TUPLES_OK) {
        if(PQntuples(exec)>0) //кол-во щаписей
            result_str=strdup(PQgetvalue(exec,0,0));
        else {
            puts("[LOG] - User not found - [get_salt]");
        }
    }
    else {
        printf("[ERROR] - Query failed: %s - [get_salt]\n",PQresultErrorMessage(exec));
    }

    PQclear(exec);
    return result_str;
}
