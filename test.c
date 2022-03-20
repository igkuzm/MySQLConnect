/**
 * File              : test.c
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 19.03.2022
 * Last Modified Date: 19.03.2022
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */

#include <stdio.h>
#include "MySQLConnect.h"

int main(int argc, char *argv[])
{
	printf("MySQL Version: %s\n", mysql_connect_print_client_version());
	getchar();
	return 0;
}
