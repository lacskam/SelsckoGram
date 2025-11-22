 #include "database_auth.h"

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
    auth_data* data=get_auth_data(username);
    if(!data)
        return -1;


    if(strcmp(hash_password(entered_password,data->salt),data->hashed_password)==0) {
        printf("[LOG] - Password verification successful for user id:%d - [verify_password]\n",data->u_id);
        free(data->salt);
        free(data->hashed_password);
        free(data);
        return 0;
    }
    else {
        puts("[ERROR] - Wrong password - [verify_password]");
        free(data->salt);
        free(data->hashed_password);
        free(data);
        return -1;
    }
}

auth_data* get_auth_data(const char *username)
{
    auth_data* data=NULL;
    if(db_connection_status()==-1)
    {
        return NULL;
    }
    const char* params[1]={username};
    PGresult *exec=PQexecParams(_connect,"SELECT user_id,salt,hashed_password FROM accounts "
                                "WHERE login=$1",1,NULL,params,NULL,NULL,0);

    if(PQresultStatus(exec)==PGRES_TUPLES_OK) {
        if(PQntuples(exec)>0) {
            data=Malloc(sizeof(auth_data));
            data->u_id=atoi(PQgetvalue(exec,0,0));
            data->salt=strdup(PQgetvalue(exec,0,1));
            data->hashed_password=strdup(PQgetvalue(exec,0,2));
        }
        else {
            puts("[LOG] - User not found - [get_salt]");
        }
    }
    else {
        printf("[ERROR] - Query failed: %s - [get_salt]\n",PQresultErrorMessage(exec));
    }

    PQclear(exec);
    return data;
}
