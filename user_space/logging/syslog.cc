#include <syslog.h> // for openlog(3), syslog(3), closelog(3)
#include <unistd.h> // for sleep(3)
#include <stdlib.h> // for system(3)
#include <stdarg.h> // for vsyslog(3)

/*
 * This example shows syslog basic usage. At the end it demostrates that the log
 * actually reached the system log.
 *
 * It seems that the openlog and closelog are not mandatory...
 * If you leave them out then you will not get your own name as the application
 * name and so you get defaults for parameters passed to openlog.
 *
 * How is syslog implemented? Either as a socket or udp socket.
 *
 * A question that is often raised is what is the performance of syslog compared to
 * printf to a file.
 *
 * TODO:
 * - show the open file descriptor table after doing openlog.
 * - show the use of vsyslog
 */

int main(int argc, char **argv, char **envp) {
	const int a = 2, b = 2;
	int c = a + b;

	openlog("driver_dlls", LOG_PID, LOG_USER);
	for (int i = 0; i < 200; i++) {
		syslog(LOG_ERR, "did you know that %d+%d=%d?", a, b, c);
	}
	sleep(1);
	int err=system("tail /var/log/syslog");
	closelog();
	return err;
}
