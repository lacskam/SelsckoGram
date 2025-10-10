#include "database_query.h"

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
