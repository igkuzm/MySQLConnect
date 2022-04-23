/**MySQLConnect.h
 * File              : MySQLConnect.c
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 06.09.2021
 * Last Modified Date: 23.04.2022
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

#define ERROR(...) ({char ___err[BUFSIZ]; sprintf(___err, __VA_ARGS__); perror(___err);})

const char*
mysql_connect_print_client_version()
{
  return mysql_get_client_info();	
}

int 
mysql_connect_execute_function(const char *address, const char *user, const char *password, const char *database, const char *sql, void *user_data,  int (*callback)(void*,int,char**,char**))
{
	MYSQL *con = mysql_init(NULL); //init mysql

	if (con == NULL){
		ERROR("mysql_init() failed\n");
		return -1;
	}

	mysql_options(con, MYSQL_SET_CHARSET_NAME, "utf8"); //set UTF8 to chars
	mysql_options(con, MYSQL_INIT_COMMAND, "SET NAMES utf8");

	if (mysql_real_connect(con, address, user, password, database, 0, NULL, 0) == NULL) {//connect to MySQL database
		ERROR("MySQL: cannot connect to  database '%s': %s\n", database, mysql_error(con));
		mysql_close(con);
		return 0;
	}

	//make SQL Query
	int res = mysql_query(con, sql);
	if (res){
		ERROR("MySQL: cannot make SQL Query '%s': %s\n", sql, mysql_error(con));
		mysql_close(con);
		return res;
	}


	MYSQL_RES *result = mysql_store_result(con);

	if (result == NULL){
		ERROR("MySQL: no data fetched from table with SQL Query '%s'\n", sql);
		mysql_close(con);
		return 0;
	}

	int num_fields = mysql_num_fields(result);
	unsigned int i;

	MYSQL_ROW row;
	MYSQL_FIELD *field;

	char *titles[num_fields];
	for (i=0; (field=mysql_fetch_field(result)); i++){
		titles[i] = field->name;
	}
	
	while ((row = mysql_fetch_row(result))){
		if (callback) {
			int c = callback(user_data, num_fields, row, titles);
			if (c) { //callback return non zero
				ERROR("MySQLExecute interupted with code: %d", c);
				break;
			}
		}
	}

	mysql_free_result(result);
	mysql_close(con);

	return 0;
}

int 
mysql_connect_callback_print(void *data, int argc, char **argv, char **columnName)
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
mysql_connect_execute(const char *address, const char *user, const char *password, const char *database, const char *sql)
{
	return mysql_connect_execute_function(address, user, password, database, sql, NULL, mysql_connect_callback_print);
}

bool 
mysql_connect_connection_to_base_is_valid(const char *server, const char *user, const char *password, const char *database)
{
	MYSQL *con = mysql_init(NULL); //init mysql

	if (con == NULL){
		ERROR("mysql_init() failed\n");
		return false;
	}

	if (mysql_real_connect(con, server, user, password, database, 0, NULL, 0) == NULL) {//connect to MySQL database
		ERROR("MySQL: cannot connect to  database '%s': %s\n", database, mysql_error(con));
		mysql_close(con);
		return false;
	}

	mysql_close(con);
	return true;
}


int 
mysql_connect_get_string_callback(void *data, int argc, char **argv, char **titles)
{
	char *string=(char*)data;
	strcpy(string, "");		
	if (argv[0]!=NULL){
		strncpy(string, (const char*)argv[0], BUFSIZ - 1); //save copy string
		string[BUFSIZ - 1] = '\0';
	}	
    return 1; //do not call callback again
}

int
mysql_connect_get_string(const char *server, const char *user, const char *password, const char *database, const char *sql, char * string)
{
	return mysql_connect_execute_function(server, user, password, database, sql, string, mysql_connect_get_string_callback);	
}

