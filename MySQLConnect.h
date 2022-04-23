/**
 * File              : MySQLConnect.h
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 06.09.2021
 * Last Modified Date: 23.04.2022
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */

#ifndef MYSQL_CONNECT
#define MYSQL_CONNECT
#include <stdbool.h>

//Print MySQL Client version
const char * mysql_connect_print_client_version();

//return true in connection is OK
bool mysql_connect_connection_to_base_is_valid(
		const char *address,  //MySQL server address
		const char *user,     //MySQL server login
		const char *password, //MySQL server password     
		const char *database  //MySQL database to connect
);

//extecute SQL Request for DataBase and run callback, return 0 if OK, otherwise return error code
int mysql_connect_execute_function(
		const char *address,  //MySQL server address
		const char *user,     //MySQL server login
		const char *password, //MySQL server password
		const char *database, //MySQL database to connect
		const char *sql,      //SQL String to execute
		void *user_data,      //pointer to transfer throught callback
		int (*callback)(      //callback finction, return non zero to stop function
			void*  user_data, //transfered pointer
			int    argc,      //number of coluns
			char** argv,      //array of arguments (cell values)
			char** titles     //array of column titles
		) 
);

//execute without callback
int mysql_connect_execute(
		const char *address,   //MySQL server address
		const char *user,      //MySQL server login
		const char *password,  //MySQL server password
		const char *database,  //MySQL database to connect
		const char *sql        //SQL String to execute
);

//get string with SQL request
int mysql_connect_get_string(
		const char *address,    //MySQL server address
		const char *user,       //MySQL server login
		const char *password,   //MySQL server password
		const char *database,   //MySQL database to connect
		const char *sql,        //SQL String to execute
		      char *string      //string to return
);

#endif /* ifndef MYSQL_CONNECT */
