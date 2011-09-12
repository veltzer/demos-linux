#include <iostream> // for std::cout, std::cerr, std::endl
#include <mysql++.h> // for mysqlpp::*

/*
 * You need all of these folders added with -I because of how the h files
 * for mysql++ are written.
 * EXTRA_LIBS=-I/usr/include/mysql++ -I/usr/include/mysql -lmysqlpp
 */

/* This is a mysql++ demo program */

int main(int argc,char** argv, char** envp) {
	// parameters are: schema, host, user, pass, use exceptions
	mysqlpp::Connection con("myworld", "localhost", "mark", "", true);
	mysqlpp::Query query=con.query("select id,name from TbOrganization order by id");
	mysqlpp::StoreQueryResult res=query.store();
	unsigned int j = 0;
	std::cout << "Records Found: " << res.size() << std::endl;
	for (mysqlpp::StoreQueryResult::iterator i = res.begin(); i != res.end(); i++) {
		mysqlpp::Row row = *i;
		std::cout << j << ":" << row["name"] << std::endl;
		j++;
	}
	return(0);
}
