#include "change_db.h"
#include <stdio.h>
#include <stdlib.h>
#include "sqlite3.h"
#include <string.h>
void change_db(char* name, char* num , char* change ,char* kind, char* rtnresult)
{       
      	printf("%s %s %s %s \n", name, num, change, kind);
	sqlite3 *db = NULL;
	int rc;
	char *zErrMsg = 0;
	char db_sql_change[500] = {0};
	rc = sqlite3_open("db.db", &db);
	if (rc)    
	{
		printf("open failed!\n");
		sqlite3_close(db);
		exit(EXIT_FAILURE);
	}
	else
	{
		printf("open success!\n");	
	}
	char order[10] = {0};
	if (strcmp(change, "dele") == 0)
	{
		if (strcmp(kind, "send") == 0)
		strcpy(order, "tras");
		else if (strcmp(kind, "recr") == 0)
		strcpy(order, "trar");
		else if (strcmp(kind, "recu") == 0)
		strcpy(order, "trau");
		else if (strcmp(kind, "draf") == 0)
		strcpy(order, "trad");
	}
	else if (strcmp(change, "retn") == 0)
	{
		if (strcmp(kind, "tras") == 0)
		strcpy(order, "send");
		else if (strcmp(kind, "trar") == 0)
		strcpy(order, "recr");
		else if (strcmp(kind, "trau") == 0)
		strcpy(order, "recu");
		else if (strcmp(kind, "trad") == 0)
		strcpy(order, "draf");
	}
	else if (strcmp(change, "read") == 0)
	{
		if (strcmp(kind, "recu") == 0)
		strcpy(order, "recr");
	}
 	printf("%s\n", order);
	int mail_num = atoi(num);	
	sprintf(db_sql_change, "UPDATE \"table_%s\" set mail_kind='%s' where mail_num=%d", name, order, mail_num);
	printf("%s\n",db_sql_change);
	sqlite3_exec (db, db_sql_change, 0, 0, &zErrMsg);

	if (zErrMsg != NULL)
	{
		strcpy(rtnresult, zErrMsg);
		printf("%s \n", rtnresult);
	}
	else 
	{
		strcpy(rtnresult, "change successful!");
	}
	sqlite3_close(db);
}
