/**
 * File              : MySQLConnect.h
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 06.09.2021
 * Last Modified Date: 19.03.2022
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */

#ifndef MYSQL_CONNECT
#define MYSQL_CONNECT
#include <stdbool.h>

//Print MySQL Client version
const char * mysql_connect_print_client_version();

bool mysql_connect_connection_to_base_is_valid(const char *address, const char *user, const char *password, const char *DataBase);

//extecute SQL Request for DataBase and run callback, return number of rows
int mysql_connect_execute_function(
		const char *address, //MySQL server address 
		const char *user, //MySQL server login
		const char *password, //MySQL server password
		const char *SQL_string, //SQL String to execute
		const char *DataBase, //MySQL database to connect
		void *user_data, //pointer to transfer throw callback
		int (*callback)( //callback finction, return non zero to stop function
			void* user_data, //transfered pointer
			int* count, //row number
			int argc, //number of coluns
			char** argv, //array of arguments (cell values)
			char** titles //array of column titles
		) 
);

//execute without callback
int mysql_connect_execute(const char *address, const char *user, const char *password, const char *SQL_string, const char *DataBase);

//return string with SQL request
char *mysql_connect_get_string(const char *address, const char *user, const char *password, const char *SQL_string, const char *DataBase);

#endif /* ifndef MYSQL_CONNECT */
