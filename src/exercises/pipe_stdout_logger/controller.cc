/*
 * This file is part of the linuxapi project.
 * Copyright (C) 2011-2013 Mark Veltzer <mark.veltzer@gmail.com>
 *
 * The linuxapi package is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * The linuxapi package is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with the GNU C Library; if not, write to the Free
 * Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 * 02111-1307 USA.
 */

#include <firstinclude.h>
#include <sys/types.h>	// for waitpid(2), open(2)
#include <sys/wait.h>	// for waitpid(2)
#include <unistd.h>	// for close(2), dup(2), execl(3), fork(2)
#include <stdlib.h>	// for EXIT_SUCCESS
#include <us_helper.h>	// for CHECK_NOT_M1()
#include <sys/stat.h>	// for open(2)
#include <fcntl.h>	// for open(2)

/*
 * This is the controller program. It launches the uncontrolled program
 * and determines where it's log will go to, does rotation of logs while
 * the uncontrolled is running etc... All of this WITHOUT copyting the logs,
 * EVER, and without missing a single log message...
 * It can also be used as a watchdog...
 */

void runUncontrolled(int* fd) {
	const char* exe="src/exercises/stdout_logger/uncontrolled.elf";
	// close standard output
	CHECK_NOT_M1(close(STDOUT_FILENO));
	// close the read end of the pipe
	CHECK_NOT_M1(close(fd[0]));
	// setup fd 1 to be correct
	CHECK_NOT_M1(dup2(fd[1], STDOUT_FILENO));
	// execute the uncontrolled process
	CHECK_NOT_M1(execl(exe, exe, NULL));
}

/*
 * Very similar to the above...
 */

static int logfile_counter=0;
int getnextlogfile() {
	// TODO: 256 is ugly - fix this
	unsigned int filename_length=256;
	char logfilename[filename_length];
	snprintf(logfilename, filename_length, "/tmp/log%d.txt", logfile_counter);
	int logfd=CHECK_NOT_M1(open(logfilename, O_WRONLY|O_CREAT|O_TRUNC, 0666));
	logfile_counter++;
	return logfd;
}

void runLogger(int* fd) {
	// close standard input
	CHECK_NOT_M1(close(STDIN_FILENO));
	// close the write end of the pipe
	CHECK_NOT_M1(close(fd[1]));
	// setup fd 1 to be correct
	CHECK_NOT_M1(dup2(fd[0], STDIN_FILENO));
	// lets log...
	int logfd=getnextlogfile();
	// buffer for the logging...
	const unsigned int bufsize=getpagesize();
	char buf[bufsize];
	unsigned int msg_count=0;
	while(true) {
		ssize_t size_read=CHECK_NOT_M1(read(STDIN_FILENO, buf, bufsize));
		if(size_read==0) {
			break;
		}
		ssize_t size_write __attribute__((unused))=CHECK_NOT_M1(write(logfd, buf, size_read));
		// TODO: we need to check that everything is written
		// I am too lazy to do this...
		if(msg_count%1000==0) {
			CHECK_NOT_M1(close(logfd));
			logfd=getnextlogfile();
		}
		msg_count++;
	}
}

int main(int argc, char** argv, char** envp) {
	int fd[2];
	CHECK_NOT_M1(pipe(fd));
	// child one
	int pid1=CHECK_NOT_M1(fork());
	if(pid1==0) {
		runUncontrolled(fd);
	}
	// child two
	int pid2=CHECK_NOT_M1(fork());
	if(pid2==0) {
		runLogger(fd);
	}
	// close the pipe at the parent
	// we cannot close before the fork or the children will not be able to use this
	// pipe...
	CHECK_NOT_M1(close(fd[0]));
	CHECK_NOT_M1(close(fd[1]));
	// wait for both children to die...
	int status;
	CHECK_NOT_M1(waitpid(pid1, &status, 0));
	CHECK_NOT_M1(waitpid(pid2, &status, 0));
	// here we can respawn the child if we want watchdog type functionality
	// too...
	return EXIT_SUCCESS;
}
