#include <postgresql/libpq-fe.h>
#include <stdio.h>
#include <stdlib.h>

/*
 * EXTRA_LIBS=-lpq
 *
 *	Mark Veltzer
 *
 */
void exit_nicely(PGconn *conn) {
	PQfinish(conn);
	exit(1);
}


int main(int argc, char** argv, char** envp) {
	char *pghost = (char *)"database";
	char *pgport = NULL;
	char *pgoptions = NULL;
	char *pgtty = NULL;
	char *dbName = (char *)"thumbnail";

	PGconn *conn = PQsetdb(pghost, pgport, pgoptions, pgtty, dbName);

	if (PQstatus(conn) == CONNECTION_BAD) {
		fprintf(stderr, "Connection to database '%s' failed.\n", dbName);
		fprintf(stderr, "%s", PQerrorMessage(conn));
		exit_nicely(conn);
	}

	PGresult *res = PQexec(conn, "BEGIN");
	if (!res || (PQresultStatus(res) != PGRES_COMMAND_OK)) {
		fprintf(stderr, "BEGIN command failed\n");
		PQclear(res);
		exit_nicely(conn);
	}
	PQclear(res);
	res = PQexec(conn, "DECLARE mycursor CURSOR FOR select * from item");
	if (!res || (PQresultStatus(res) != PGRES_COMMAND_OK)) {
		fprintf(stderr, "DECLARE CURSOR command failed\n");
		PQclear(res);
		exit_nicely(conn);
	}
	PQclear(res);
	res = PQexec(conn, "FETCH ALL in mycursor");
	if (!res || (PQresultStatus(res) != PGRES_TUPLES_OK)) {
		fprintf(stderr, "FETCH ALL command didn't return tuples properly\n");
		PQclear(res);
		exit_nicely(conn);
	}
	int nFields = PQnfields(res);
	for (int i = 0; i < nFields; i++) {
		printf("%-15s", PQfname(res, i));
	}
	printf("\n\n");
	for (int i = 0; i < PQntuples(res); i++) {
		for (int j = 0; j < nFields; j++) {
			printf("%-15s", PQgetvalue(res, i, j));
		}
		printf("\n");
	}
	PQclear(res);
	res = PQexec(conn, "CLOSE mycursor");
	PQclear(res);
	res = PQexec(conn, "COMMIT");
	PQclear(res);
	PQfinish(conn);
	return EXIT_SUCCESS;
}
