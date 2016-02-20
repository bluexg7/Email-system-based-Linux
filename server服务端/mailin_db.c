#include <stdio.h>
#include <stdlib.h>
#include "sqlite3.h"
#include <string.h>
#include "mailin_db.h"
void mailin_db(char* name, char* kind, char* send, char* rece, char *time, char* text, char* info, char* rtnresult)
{       
      
	sqlite3 *db = NULL;
	int rc = 0;
	char *zErrMsg = 0;
	char db_sql_insert[3000] = {0};
	
	int row = 0, column = 0;
//open database
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
//get time 
	sprintf(db_sql_insert, "SELECT datetime('now');");
	char **azResult;	
	sqlite3_get_table(db,db_sql_insert, &azResult, &row, &column, &zErrMsg);
	strcpy(time, (char*)azResult[1]);
	sqlite3_free_table(azResult);
//insert in text   table

	strcpy(db_sql_insert, "SELECT * FROM mail_text");
	sqlite3_get_table(db, db_sql_insert, &azResult, &row, &column, &zErrMsg);
	int text_num = 0;
	text_num = row + 1;
	char t_num[20] = {0};
	sprintf(t_num, "%d", text_num);
	sprintf(db_sql_insert, "INSERT INTO \"mail_text\" VALUES(%d,'%s','%s','%s');",text_num, text, info, kind);
	sqlite3_exec (db, db_sql_insert, 0, 0, &zErrMsg);
	if (zErrMsg != NULL)
	{
		strcpy(rtnresult, zErrMsg);
		printf("%s\n", zErrMsg);
	}
	else 
	{
		printf("insert in mail_text database successfully!\n");
	}
	memset(db_sql_insert,0,3000);
	sqlite3_free_table(azResult);
//insert in sender table
	char table[50] = {0};	
	char inkind[10] = {0};
	char mail_info[200] = {0};
	if (strcmp (rece, "indraf") == 0)//judge if is indraft 
	{
		strcpy(inkind, "draf");
	}
	else 
	{
		strcpy(inkind, "send");	
	}
	sprintf(table, "table_%s",send);
	sprintf(db_sql_insert,"SELECT * FROM %s",table);
	sqlite3_get_table(db, db_sql_insert, &azResult, &row, &column, &zErrMsg);
	int num = 0;
	num = row + 1;
	sprintf(mail_info,"%d",num);
	
	sprintf(db_sql_insert, "INSERT INTO \"%s\" VALUES(%d,'%s','%s','%s','%s','%s','%s','%s');", table, num, name, inkind, send, rece, time, t_num, mail_info);
	sqlite3_exec (db, db_sql_insert, 0, 0, &zErrMsg);
	if (zErrMsg != NULL)
	{
		strcpy(rtnresult, zErrMsg);
		printf("%s\n", rtnresult);
	}
	else 
	{
		printf("insert in %s database successfully!\n", table);
		//printf("insert data:  %d,'%s','%s','%s','%s','%s','%s','%s' \n", num, name, inkind, send, rece, time, t_num, info);
	}
	sqlite3_free_table(azResult);
//insert in recever table
	if (strcmp (inkind, "draf") != 0)
{	strcpy(inkind, "recu");
	char recever[30] = {0};
	int i = 0, j = 0, k = 0;
	for ( k = 0; k < 10; k++)// only 10 recevers one time!!! 
	{	int nrow = 0;
		for (; i < strlen(rece); )
		{
			if (rece[i] == ';')
			{
				j = 0;
				i ++;
				break;	
			}
			//printf(" %s  i = %d  j = %d \n", recever, i, j);
			recever[j++] = rece[i++];
		}
		memset(db_sql_insert,0,3000);
		sprintf(table, "table_%s",recever);
		sprintf(db_sql_insert,"SELECT * FROM %s",table);
		sqlite3_get_table(db, db_sql_insert, &azResult, &nrow, &column, &zErrMsg);
		printf("in receiver row:%d\n",nrow);
		num = nrow +1 ;
		sprintf(mail_info,"%d",num);
		sprintf(db_sql_insert, "INSERT INTO \"%s\" VALUES(%d,'%s','%s','%s','%s','%s','%s','%s');", table, num, name, inkind, send, rece, time, t_num, mail_info);
		sqlite3_exec (db, db_sql_insert, 0, 0, &zErrMsg);
		if (zErrMsg != NULL)
		{
			printf("%s\n", zErrMsg);
			strcpy(rtnresult, zErrMsg);
		}	
		else 
		{
			printf("insert in %s database successfully!\n", table);
			strcpy(rtnresult, "\n insert database successfully!");
			printf("insert data:  %d,'%s','%s','%s','%s','%s','%s','%s' \n", num, name, inkind, send, rece, time, t_num, mail_info);
		}
		if (i == strlen(rece))
		{
			break;	
		}
		memset(recever,0,30);
		sqlite3_free_table(azResult);
	}
}	
	sqlite3_free_table(azResult);
	sqlite3_close(db);
}

