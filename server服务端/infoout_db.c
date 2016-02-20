#include <stdio.h>
#include <stdlib.h>
#include "sqlite3.h"
#include <string.h>
#include "infoout_db.h"
 typedef struct e_rec
{
	char topic[50];
	char kind[10];
	char sender[20];
	char reciever[200];
	char time[20];
	char textnum[20];
	char info[50];
}E_rec;
 extern E_rec part[200];
void db_infoout(char *name, char *kind, char *rtnresult, int *num) 

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
	if (strcmp (kind, "recv") == 0)
	{
		sprintf(db_select_sql,"SELECT * FROM table_%s where mail_kind LIKE 'rec_'", name);
	}
	else if(strcmp (kind, "send") == 0)
	{
		sprintf(db_select_sql,"SELECT * FROM table_%s where mail_kind = 'send'", name);	
	}
	else if(strcmp (kind, "draf") == 0)
	{
		sprintf(db_select_sql,"SELECT * FROM table_%s where mail_kind = 'draf'", name);	
	}
	else if(strcmp (kind, "tras") == 0)
	{
		sprintf(db_select_sql,"SELECT * FROM table_%s where mail_kind LIKE 'tra_' ", name);	
	}
	printf("%s\n", db_select_sql);			
	sqlite3_get_table(db, db_select_sql, &azResult, &row, &column, &zErrMsg);
	*num = row ;
	int i = 0;
	//for(i = 0; i < (row + 1) * column; i++)
	//{
	//	printf("result %d : %s \n", i, azResult[i + 1]);
	//}
	for( i = 0; i < row ; i++)
	{	
		strcpy(part[i].topic ,azResult[(i + 1) * column + 1]);
		strcpy(part[i].kind ,azResult[(i + 1) * column + 2]);
		strcpy(part[i].sender ,azResult[(i + 1) * column + 3]);
		strcpy(part[i].reciever ,azResult[(i + 1) * column + 4]);
		strcpy(part[i].time ,azResult[(i + 1) * column + 5]);
		strcpy(part[i].textnum ,azResult[(i + 1) * column + 6]);
		strcpy(part[i].info ,azResult[(i + 1) * column + 7]);
	}
	if (zErrMsg == NULL)	
	{		
		strcpy(rtnresult, "select successful!\n");
	}
	else
	{
		strcpy(rtnresult, zErrMsg);
	}	
	sqlite3_free_table(azResult);
	sqlite3_close (db);
}

