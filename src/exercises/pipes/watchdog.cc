/*
        This file is part of the linuxapi project.
        Copyright (C) 2011-2013 Mark Veltzer <mark.veltzer@gmail.com>

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

#include <firstinclude.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>	// for EXIT_SUCCESS
#include <us_helper.h>	// for CHECK_NOT_M1()

struct itimerval timer;
int pipefd[2];
int gotusr1=0;
int gotusr2=0;

int child1pid, child2pid;

void sigIntHandler(int gotsig) {
	kill(child1pid, SIGKILL);
	kill(child2pid, SIGKILL);
	CHECK_NOT_M1(unlink("np"));
	exit(0);
}

void sigChildHandler(int gotsig) {
	int status;
	pid_t pid;
	while((pid=wait3(&status, WNOHANG, NULL))>0) {
		if(WIFEXITED(status))
			printf("Child %d exited. Status: %d\n", pid, WEXITSTATUS(status));
		if(WIFSIGNALED(status))
			printf("Child %d killed. Signal: %d\n", pid, WTERMSIG(status));
	}
}

void sigUsrxHandler(int gotsig) {
	time_t now;
	char* stime;
	now=time(NULL);
	stime=ctime(&now);
	stime[strlen(stime)-1]='\0';
	switch(gotsig) {
	case SIGUSR1:
		gotusr1=1;
		printf("%s: TTL from 1\n", stime);
		break;
	case SIGUSR2:
		gotusr2=1;
		printf("%s: TTL from 2\n", stime);
		break;
	}
	if(gotusr1 && gotusr2) {
		CHECK_NOT_M1(setitimer(ITIMER_REAL, &timer, NULL));
		gotusr1=gotusr2=0;
	}
}

void doChild(const char * chld)
{
	execl("./child", "./child", chld, NULL);
	fprintf(stderr, "execl child %s failed: %s\n", chld, strerror(errno));
	exit(errno);
}

void startChild1()
{
	child1pid=CHECK_NOT_M1(fork());
	// child
	if(child1pid==0) {
		close(pipefd[0]);
		dup2(pipefd[1], 1);
		close(pipefd[1]);
		doChild("1");
		exit(0);
	}
}

void startChild2()
{
	child2pid=CHECK_NOT_M1(fork());
	// child
	if(child2pid==0) {
		close(pipefd[1]);
		dup2(pipefd[0], 0);
		close(pipefd[0]);
		doChild("2");
		exit(0);
	}
}

void sigAlrmHandler(int gotsig) {
	if(gotusr1==0) {
		printf("Signal form child 1 ID %d lost. Restarting\n", child1pid);
		kill(child1pid, SIGKILL);
		startChild1();
	}
	if(gotusr2==0) {
		printf("Signal from child 2 ID %d lost. Restarting\n", child2pid);
		kill(child2pid, SIGKILL);
		startChild2();
	}
}

int main(int argc, char** argv, char** envp) {
	struct sigaction sigusr, sigchld, sigalrm, sigint;
	sigset_t emptyset;
	mkfifo("np", 0666);
	sigemptyset(&emptyset);
	sigusr.sa_handler=sigUsrxHandler;
	sigusr.sa_mask=emptyset;
	sigusr.sa_flags=0;
	CHECK_NOT_M1(sigaction(SIGUSR1, &sigusr, NULL));
	CHECK_NOT_M1(sigaction(SIGUSR2, &sigusr, NULL));
	sigchld.sa_handler=sigChildHandler;
	sigchld.sa_mask=emptyset;
	sigchld.sa_flags=0;
	CHECK_NOT_M1(sigaction(SIGCHLD, &sigchld, NULL));
	sigalrm.sa_handler=sigAlrmHandler;
	sigalrm.sa_mask=emptyset;
	sigalrm.sa_flags=0;
	CHECK_NOT_M1(sigaction(SIGALRM, &sigalrm, NULL));
	sigint.sa_handler=sigIntHandler;
	sigint.sa_mask=emptyset;
	sigint.sa_flags=0;
	CHECK_NOT_M1(sigaction(SIGINT, &sigint, NULL));
	timer.it_interval.tv_sec=5;
	timer.it_interval.tv_usec=0;
	timer.it_value.tv_sec=5;
	timer.it_value.tv_usec=0;
	CHECK_NOT_M1(setitimer(ITIMER_REAL, &timer, NULL));
	CHECK_NOT_M1(pipe(pipefd));
	startChild1();
	startChild2();
	while(true) {
		sigsuspend(&emptyset);
	}
	return EXIT_SUCCESS;
}
