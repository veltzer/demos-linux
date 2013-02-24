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
#include <unistd.h> // for fork(2)
#include <stdio.h> // for fgets(3)
#include <sys/types.h> // for waitid(2)
#include <sys/wait.h> // for waitid(2)
#include <stdlib.h> // for exit(3), atoi(3), EXIT_SUCCESS, EXIT_FAILURE
#include <string.h> // for strsignal(3)
#include <us_helper.h> // for TRACE(), CHECK_NOT_M1()

/*
* This example explains how parents should wait for their children
* and analyze the result of the childs death or the like.
* The example uses the most complex interface waitid(2). You could,
* ofcourse, alter it to use one of the simpler apis:
* wait, waitpid, wait3, wait4...
*/

void print_status(int status) {
	if (WIFEXITED(status)) {
		TRACE("child exited normally with status %d", WEXITSTATUS(status));
	}
	if (WIFSTOPPED(status)) {
		TRACE("child was stopped with signal %s", strsignal(WSTOPSIG(status)));
	}
	if (WIFSIGNALED(status)) {
		TRACE("child was signaled with signal %s", strsignal(WTERMSIG(status)));
	}
}


void print_code(int code) {
	switch (code) {
	case CLD_EXITED:
		TRACE("child exited of it's own accord");
		break;

	case CLD_KILLED:
		TRACE("child was killed");
		break;

	case CLD_STOPPED:
		TRACE("child was stopped");
		break;

	case CLD_CONTINUED:
		TRACE("child was continued");
		break;
	}
}

int main(int argc,char** argv,char** envp) {
	TRACE("this is the parent");
	pid_t child_pid=CHECK_NOT_M1(fork());
	if(child_pid==0) {
		bool selected=false;
		int selection;
		while(!selected) {
			TRACE("this is the child");
			TRACE("Please select what do you want to do:");
			TRACE("1) exit(EXIT_SUCCESS)");
			TRACE("2) exit(EXIT_FAILURE)");
			TRACE("3) do SIGSEGV");
			TRACE("- do 'kill -s SIGSTOP %d'", getpid());
			TRACE("- do 'kill -s SIGCONT %d'", getpid());
			char str[256];
			CHECK_CHARP(fgets(str, 256, stdin),str);
			selection=atoi(str);
			if ((selection>=1) && (selection<=3)) {
				selected=true;
			}
		}
		switch (selection) {
			case 1:
				exit(EXIT_SUCCESS);
				break;

			case 2:
				exit(EXIT_FAILURE);
				break;

			case 3:
				*((char *)0)=0;
				break;
		}
	} else {
		TRACE("this is the parent");
		bool over=false;
		while(!over) {
			TRACE("waiting for the child...");
			siginfo_t info;
			CHECK_NOT_M1(waitid(P_PID, child_pid, &info, WEXITED | WSTOPPED | WCONTINUED));
			print_code(info.si_code);
			print_status(info.si_status);
			if ((info.si_code==CLD_EXITED) || (info.si_code==CLD_KILLED)) {
				over=true;
			}
		}
	}
	return EXIT_SUCCESS;
}
