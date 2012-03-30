#include<syslog.h> // for openlog(3), syslog(3), closelog(3)
#include<stdlib.h> // for EXIT_SUCCESS

/* This is a test for syslog(3) calls
 *
 *	Mark Veltzer
 *
 */

int main(int argc, char** argv, char** envp) {
	openlog(argv[0], LOG_CONS | LOG_NDELAY | LOG_PERROR | LOG_PID, LOG_USER);
	syslog(LOG_DEBUG, "test message with value [%d]", 1972);
	closelog();
	return EXIT_SUCCESS;
}
