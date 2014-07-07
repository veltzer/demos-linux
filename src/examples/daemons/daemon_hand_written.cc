/*
 * This file is part of the linuxapi package.
 * Copyright (C) 2011-2014 Mark Veltzer <mark.veltzer@gmail.com>
 *
 * linuxapi is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * linuxapi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with linuxapi. If not, see <http://www.gnu.org/licenses/>.
 */

#include <firstinclude.h>
#include <syslog.h>	// for openlog(3), syslog(3), closelog(3), setlogmask(3)
#include <unistd.h>	// for pause(2), getpid(2), getppid(2), setsid(2), fork(2),
			// chdir(2), close(2), getdtablesize(2), dup(2)
#include <stdlib.h>	// for EXIT_SUCCESS, exit(3)
#include <stdio.h>	// for printf(3)
#include <sys/types.h>	// for open(2)
#include <sys/stat.h>	// for open(2)
#include <fcntl.h>	// for open(2)
#include <err_utils.h>	// for CHECK_NOT_M1(), CHECK_ASSERT()

/*
 * This is an example of writing a daemon by yourself without
 * using the glibc daemon(3) support function...
 */

void my_daemon() {
	// let parent die and get adopted by init(1)...
	if(CHECK_NOT_M1(fork())) {
		exit(EXIT_SUCCESS);
	}
	// become session leader...
	CHECK_NOT_M1(setsid());
	// avoid 'unable to unmount' errors...
	CHECK_NOT_M1(chdir("/"));
	// close all files...
	int fdTableSize = getdtablesize();
	for(int i=0; i<fdTableSize; i++)
		CHECK_NOT_M1(close(i));
	// open standard streams to the null device
	// in case someone writes to them...
	CHECK_ZERO(open("/dev/null", O_RDWR));
	CHECK_NOT_M1(dup2(0,1));
	CHECK_NOT_M1(dup2(0,2));
}

int main(int argc, char** argv, char** envp) {
	printf("before daemon(3), pid is [%d], ppid is [%d]\n", getpid(), getppid());
	printf("after the console returns the process will continue disconnected\n");
	printf("from the terminal as child of pid 1\n");
	printf("use [ps -f -p %d] to see it's data...\n", getpid());
	printf("use [tail -f /var/log/syslog] to see the logs of this process.\n");
	my_daemon();
	// from this point prints do not get anywhere (/dev/null redirection).
	// so we use syslog instead...
	openlog(argv[0], LOG_PID, LOG_USER);
	syslog(LOG_ERR, "after daemon(3), pid is [%d], ppid is [%d]\n", getpid(), getppid());
	closelog();
	while(true) {
		int ret=pause();
		CHECK_ASSERT(ret==-1 && errno==EINTR);
	}
	return EXIT_SUCCESS;
}
