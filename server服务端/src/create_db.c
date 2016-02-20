#include <stdio.h>
#include <stdlib.h>
#include "sqlite3.h"

#define _DEBUG_
 void db_create(char *db_name, char* db_sql_create)
{
	sqlite3 *db = NULL;
	char *zErrMsg = 0;
	int rc;
	rc = sqlite3_open(db_name, &db);
	if (rc)
	{
		printf(stderr, "can't open database: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		exit(EXIT_FAILURE);
	}
	else
	{
		printf("create database %s sucessfully!\n", db_name);
	}
	sqlite3_exec(db, db_sql_create, 0, 0, &zErrMsg);
	printf("create table  successfully!\n");
#ifdef _DEBUG_
	printf("zErrMsg = %s\n", zErrMsg);
#endif
	sqlite3_close(db);
}


int main(int argc, char* argv[])
{		
	
	db_create("db.db"," CREATE TABLE user_info(\
	user_name  VARCHAR(20) PRIMARY KEY,\
	user_psd   VARCHAR(20),\
	user_phone VARCHAR(20));");	
	
	return 0;
}
