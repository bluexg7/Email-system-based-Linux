#include "insert_db.h"
#include <stdio.h>
#include <stdlib.h>
#include "sqlite3.h"
#include <string.h>
void db_insert(char* name, char* psd, char* phone, char* rtnresult)
{       
      
	sqlite3 *db = NULL;
	int rc;
	char *zErrMsg = 0;
	char db_sql_insert[500] = {0};

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
	sprintf(db_sql_insert, "INSERT INTO \"user_info\" VALUES(\
	'%s','%s','%s');", name, psd, phone);
	sqlite3_exec (db, db_sql_insert, 0, 0, &zErrMsg);
	if (zErrMsg != NULL)
	{
		if (strcmp(zErrMsg,"column user_name is not unique") == 0)
		strcpy(rtnresult, "user_name was used!");
		else
		strcpy(rtnresult, zErrMsg);
	}
	else 
	{
		
		printf("insert data: %s, %s, %s \n",name, psd, phone);
		char create_table_sql[500] = {0};
		char table_name[50] = {0};
		sprintf(table_name, "table_%s", name);
		printf("%s\n", table_name);
		sprintf(create_table_sql, "CREATE TABLE %s (\
		mail_num  INTEGER    PRIMARY KEY,\
		mail_name VARCHAR(50),\
		mail_kind VARCHAR(10),\
		mail_send VARCHAR(20),\
		mail_rece VARCHAR(200),\
		mail_time VARCHAR(25),\
		text_num  VARCHAR(20),\
		mail_info VARCHAR(200));",table_name);
		sqlite3_exec(db, create_table_sql, 0, 0, &zErrMsg);
		if (zErrMsg == NULL)
		{
			strcpy(rtnresult, "insert success!");
			printf("insert success!\n");
		}
		else
		{
			strcpy(rtnresult, zErrMsg);
			printf("%s\n", zErrMsg);
		}
	}
	sqlite3_close(db);
}

