#include <syslog.h> // for openlog(3), syslog(3), closelog(3)
#include <unistd.h> // for sleep(3)
#include <stdlib.h> // for system(3)

/*
 * This example shows syslog basic usage
 *
 * It seems that the open and close are not mandatory...
 */

int main(int argc, char **argv, char **envp) {
	const int a = 2, b = 2;
	int c = a + b;

	openlog("driver_dlls", LOG_PID, LOG_USER);
	for (int i = 0; i < 200; i++) {
		syslog(LOG_ERR, "did you know that %d+%d=%d?", a, b, c);
	}
	closelog();
	sleep(1);
	int err=system("tail /var/log/syslog");
	return err;
}
