#include <mysql/mysql.h>
//#include <stdlib.h>
#include <iostream>

/* This is a demo program for regular C mysql access */

/* TODO:
 * In this file the three parameters: database name,
 * database user and the password are hardcoded. Get
 * them out of here to some outside xml options file
 *
 * EXTRA_LIBS=-lmysqlclient
 */
int main(int argc, char **argv, char **envp) {
	MYSQL mysql;
	MYSQL *ret0 = mysql_init(&mysql);

	if (!ret0) {
		std::cout << "init failed\n";
		std::cout << mysql_error(&mysql) << "\n";
		return(1);
	}
	std::cout << "after init\n";
	MYSQL *ret1 = mysql_real_connect(&mysql, "database", "master", "master", "mysql", 0, NULL, 0);
	if (!ret1) {
		std::cout << "connect failed\n";
		std::cout << mysql_error(&mysql) << "\n";
		return(1);
	}
	std::cout << "after connect\n";
	const char *info = mysql_get_server_info(&mysql);
	if (!info) {
		std::cout << "get_server_info failed\n";
		std::cout << mysql_error(&mysql) << "\n";
		return(1);
	}
	std::cout << "info is " << info << "\n";
	mysql_close(&mysql);                                                                                                         //there are no error codes for this one
	return(0);
}
