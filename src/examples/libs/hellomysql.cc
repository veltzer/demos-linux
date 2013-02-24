/*
        This file is part of the linuxapi project.
        Copyright (C) 2011-2013 Mark Veltzer <mark.veltzer@gmail.com>

        The linuxapi package is free software; you can redistribute it and/or
        modify it under the terms of the GNU Lesser General Public
        License as published by the Free Software Foundation; either
        version 2.1 of the License, or (at your option) any later version.

        The linuxapi package is distributed in the hope that it will be useful,
        but WITHOUT ANY WARRANTY; without even the implied warranty of
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
        Lesser General Public License for more details.

        You should have received a copy of the GNU Lesser General Public
        License along with the GNU C Library; if not, write to the Free
        Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
        02111-1307 USA.
 */

#include <firstinclude.h>
#include <mysql/mysql.h>
#include <iostream>	// for std::cout, std::endl, std::cerr
#include <stdlib.h>	// for EXIT_SUCCESS

/*
 * This is a demo program for regular C mysql access
 *
 * TODO:
 * In this file the three parameters: database name,
 * database user and the password are hardcoded. Get
 * them out of here to some outside xml options file
 *
 * EXTRA_LINK_FLAGS=-lmysqlclient
 */

int main(int argc, char** argv, char** envp) {
	MYSQL mysql;
	MYSQL *ret0=mysql_init(&mysql);
	if (!ret0) {
		std::cerr << "init failed" << std::endl;
		std::cerr << mysql_error(&mysql) << std::endl;
		return EXIT_FAILURE;
	}
	std::cout << "after init" << std::endl;
	MYSQL *ret1=mysql_real_connect(&mysql, "database", "master", "master", "mysql", 0, NULL, 0);
	if (!ret1) {
		std::cerr << "connect failed" << std::endl;
		std::cerr << mysql_error(&mysql) << std::endl;
		return EXIT_FAILURE;
	}
	std::cout << "after connect" << std::endl;
	const char *info=mysql_get_server_info(&mysql);
	if (!info) {
		std::cerr << "get_server_info failed" << std::endl;
		std::cerr << mysql_error(&mysql) << std::endl;
		return EXIT_FAILURE;
	}
	std::cout << "info is " << info << std::endl;
	mysql_close(&mysql);	// there are no error codes for this one
	return EXIT_SUCCESS;
}
