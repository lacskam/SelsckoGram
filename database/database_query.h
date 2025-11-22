#ifndef DATABASE_QUERY_H
#define DATABASE_QUERY_H

#include "database_connect.h"

char* base_query(const char* query,int params_number, const char** params);

#endif
