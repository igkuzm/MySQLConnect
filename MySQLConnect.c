/**MySQLConnect.h
 * File              : MySQLConnect.c
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 06.09.2021
 * Last Modified Date: 19.03.2022
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */

#include "MySQLConnect.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef __APPLE__
	#include <TargetConditionals.h>
	#if TARGET_IPHONE_SIMULATOR
		#include "mysqlclient/iPhoneSimulator/include/mysql.h"
	#elif TARGET_OS_MACCATALYST
	#elif TARGET_OS_IPHONE
		#include "mysqlclient/iOS/include/mysql.h"
	#elif TARGET_OS_MAC
		#include "mysqlclient/MacOS/include/mysql.h"
	#else
	#endif
#elif defined _WIN32 || defined _WIN64
	#include "mysqlclient/Windows/include/mysql.h"
#elif defined __ANDROID__
	#include "mysqlclient/Android/include/mysql.h"
#else
	#include <mysql/mysql.h>
#endif	

const char*
mysql_connect_print_client_version()
{
  return mysql_get_client_info();	
}

int 
mysql_connect_execute_function(const char *server, const char *user, const char *password, const char *String, const char *DataBase, void *data,  int (*callback)(void*,int*,int,char**,char**))
{
	MYSQL *con = mysql_init(NULL); //init mysql

	if (con == NULL){
		fprintf(stderr, "ERROR. mysql_init() failed\n");
		return 0;
	}

	mysql_options(con, MYSQL_SET_CHARSET_NAME, "utf8"); //set UTF8 to chars
	mysql_options(con, MYSQL_INIT_COMMAND, "SET NAMES utf8");

	if (mysql_real_connect(con, server, user, password, DataBase, 0, NULL, 0) == NULL) {//connect to MySQL database
		fprintf(stderr, "ERROR. Cannot connect to SQL DataBase:%s\n", DataBase);
		mysql_close(con);
		return 0;
	}

	//make SQL Query
	int err = mysql_query(con, String);
	if (err){
		fprintf(stderr, "ERROR. Cannot make SQL Query: %s, with error: %d\n", String, err);
		mysql_close(con);
		return 0;
	}


	MYSQL_RES *result = mysql_store_result(con);

	if (result == NULL){
		fprintf(stderr, "ERROR. No data fetched from table\n");
		mysql_close(con);
		return 0;
	}

	int num_fields = mysql_num_fields(result);
	unsigned int i;

	MYSQL_ROW row;
	MYSQL_FIELD *field;

	char *headers[num_fields];
	for (i=0; (field=mysql_fetch_field(result)); i++){
		headers[i] = field->name;
	}
	
	int count = 0;
	while ((row = mysql_fetch_row(result))){
		if (callback) {
			int c = callback((void*)data, &count, num_fields, row, headers);
			if (c) { //callback return non zero
				fprintf(stderr, "MySQLExecute interupted with code: %d", c);
				break;
			}
		}
		count++;
	}

	mysql_free_result(result);
	mysql_close(con);
	return count;
}

int 
mysql_connect_callback_print(void *data, int *count, int argc, char **argv, char **columnName)
{
    int i;
	for (i=0; i< argc; i++)
		if (argv[i] != NULL){
			printf("%s,\t", argv[i]);
		}
		else {
			printf("%s,\t", "(null)");
		}
    printf("\n");
    return 0;
}

int 
mysql_connect_execute(const char *address, const char *user, const char *password, const char *SQL_string, const char *DataBase)
{
	return mysql_connect_execute_function(address, user, password, SQL_string, DataBase, NULL, mysql_connect_callback_print);
}

bool 
mysql_connect_connection_to_base_is_valid(const char *server, const char *user, const char *password, const char *DataBase)
{
	MYSQL *con = mysql_init(NULL); //init mysql

	if (con == NULL){
		fprintf(stderr, "ERROR. mysql_init() failed\n");
		return false;
	}

	mysql_options(con, MYSQL_SET_CHARSET_NAME, "utf8"); //set UTF8 to chars
	mysql_options(con, MYSQL_INIT_COMMAND, "SET NAMES utf8");

	if (mysql_real_connect(con, server, user, password, DataBase, 0, NULL, 0) == NULL) {//connect to MySQL database
		fprintf(stderr, "ERROR. Cannot connect to SQL DataBase:%s\n", DataBase);
		mysql_close(con);
		return false;
	}

	mysql_close(con);
	return true;
}


int 
mysql_connect_get_string_callback(void *data, int *count, int argc, char **argv, char **columnNames)
{
	char *string=(char*)data;
	strcpy(string, "");		
	if (argv[0]!=NULL){
		char buf[BUFSIZ];		
		strncpy(buf, (const char*)argv[0], sizeof(buf) - 1); //save copy string
		buf[sizeof(buf) - 1] = '\0';
		strcpy(string, buf);
	}	
    return 1; //do not call callback again
}

char *
mysql_connect_get_string(const char *server, const char *user, const char *password, const char *SQL, const char *DataBase)
{
	char *string = calloc(BUFSIZ, sizeof(char));
	if (!string) {
		fprintf(stderr, "ERROR. Cannot allocate memory\n");		
		exit(EXIT_FAILURE);
	}
	mysql_connect_execute_function(server, user, password, SQL, DataBase, string, mysql_connect_get_string_callback);	
	return string;
}

