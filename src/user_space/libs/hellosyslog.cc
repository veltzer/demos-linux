/* This is a test for syslog(3) calls */

#include <syslog.h>

int main(int argc, char **argv) {
	openlog(argv[0], LOG_CONS | LOG_NDELAY | LOG_PERROR | LOG_PID, LOG_USER);
	syslog(LOG_DEBUG, "test message with value [%d]", 1972);
	closelog();
	return(0);
}
