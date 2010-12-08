#include <syslog.h> // for openlog(3), syslog(3), closelog(3)
#include <unistd.h> // for sleep(3)
#include <stdarg.h> // for vsyslog(3)
#include <sys/types.h> // for getpid(2)
#include <unistd.h> // for getpid(2)

#include "us_helper.hh" // for my_system

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
 * printf to a file - the answer is faster since it is sending the data through the socket
 * and does not wait for it to be written to disk.
 *
 * See a different example of how to use syslog as a general logging and tracing facility
 * using the vsyslog function.
 *
 * 					Mark Veltzer
 */

int main(int argc, char **argv, char **envp) {
	const char* myname="thisismyname";
	openlog(myname, LOG_PID, LOG_USER);
	// show the open file descriptor map after opening syslog
	// at this point the socket is still not created...
	my_system("ls -l /proc/%d/fd",getpid());
	// lets send different messages...
	for (int i = 0; i < 200; i++) {
		syslog(LOG_ERR, "did you know that %d+%d=%d?", i, i, i+i);
	}
	// lets send the same message
	for (int i = 0; i < 200; i++) {
		syslog(LOG_ERR, "did you know that 2+2=4?");
	}
	// show the open file descriptor map after opening syslog
	// the socket is now here - it seems that syslog is lazy...
	my_system("ls -l /proc/%d/fd",getpid());
	sleep(1); // let syslog a chance to actually put the data in the log
	//my_system("tail /var/log/syslog | grep %s | grep %d",myname,getpid());
	my_system("tail /var/log/syslog");
	closelog();
	return 0;
}
