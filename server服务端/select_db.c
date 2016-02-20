#include <stdio.h>
#include <stdlib.h>
#include "sqlite3.h"
#include <string.h>
#include "select_db.h"
#define _DEBUG_
void db_select(char *name, char *psd, char *rtnresult) 

{
	int row = 0, column = 0;
	char **azResult;
	char *zErrMsg = 0;
	int rc;
	sqlite3 *db = NULL;
	rc = sqlite3_open("db.db", &db);
	if (rc)
	{
		fprintf (stderr, "open database failed:%s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		exit(EXIT_FAILURE);
	}
	else
	{
		printf("open success!\n");	
	}
	char db_select_sql[150] = "SELECT * FROM user_info";
	sprintf(db_select_sql,"SELECT * FROM user_info where user_name = '%s'", name);
	printf("%s\n", db_select_sql);			
	sqlite3_get_table(db, db_select_sql, &azResult, &row, &column, &zErrMsg);
/*	
	for(int i = 3; i < (row + 1) * column; i++)
	{
		printf("result %d : %s \n", i, azResult[i]);
	}
*/	
	if (row == 0)// no user in db
	{
		strcpy(rtnresult, "no user name!");
	}
	else
	{	
		
		printf("password is : %s\n", azResult[4]);
		if (strcmp(azResult[4], psd) == 0)
		{
			strcpy(rtnresult, "login success!");
		}	
		else 
		{
			strcpy(rtnresult, "password wrong!");
		}
	}
	sqlite3_free_table(azResult);
	sqlite3_close (db);
#ifdef _DEBUG_
	printf("zErrMsg = %s\n", zErrMsg);
#endif
}

