#include <stdio.h>
#include <stdlib.h>
#include "sqlite3.h"
#include <string.h>
#include "textout_db.h"
extern char bigbuf[4096];
void textout_db(char *text_num, char *rtnresult) 

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
	char db_select_sql[150] = {0};
	
	sprintf(db_select_sql,"SELECT * FROM mail_text where text_num = '%s'", text_num);	
		
	sqlite3_get_table(db, db_select_sql, &azResult, &row, &column, &zErrMsg);
	if (zErrMsg != NULL)
	{	
		strcpy(rtnresult,zErrMsg);
	}
	else
	{
		strcpy(rtnresult, "get text successfully!");
		printf("%s \n", bigbuf);
	}
	if (row != 0)
	{	
		//strcpy(bigbuf,azResult[1]);
		sprintf(bigbuf,"%s#%s",azResult[6],azResult[5]);
	}
	else
	{
		strcpy(rtnresult,"no email here!");
	}	
	sqlite3_free_table(azResult);
	sqlite3_close (db);
	
}

