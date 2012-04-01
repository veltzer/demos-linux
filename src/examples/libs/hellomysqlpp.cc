/*
	This file is part of the linuxapi project.
	Copyright (C) 2011, 2012 Mark Veltzer <mark.veltzer@gmail.com>

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

#include<iostream> // for std::cout, std::cerr, std::endl
#include<mysql++.h> // for mysqlpp::*
#include<stdlib.h> // for EXIT_SUCCESS

/*
 * This is a mysql++ demo program
 * You need all of these folders added with -I because of how the h files
 * for mysql++ are written.
 * EXTRA_LIBS=-I/usr/include/mysql++ -I/usr/include/mysql -lmysqlpp
 *
 */

int main(int argc,char** argv,char** envp) {
	// parameters are: schema, host, user, pass, use exceptions
	mysqlpp::Connection con("myworld", "localhost", "mark", "", true);
	mysqlpp::Query query=con.query("select id,name from TbOrganization order by id");
	mysqlpp::StoreQueryResult res=query.store();
	unsigned int j = 0;
	std::cout << "Records Found: " << res.size() << std::endl;
	for(mysqlpp::StoreQueryResult::iterator i=res.begin();i!=res.end();i++) {
		mysqlpp::Row row=*i;
		std::cout << j << ":" << row["name"] << std::endl;
		j++;
	}
	return EXIT_SUCCESS;
}
