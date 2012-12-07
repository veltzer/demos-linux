/*
	This file is part of the linuxapi project.
	Copyright (C) 2011, 2012 Mark Veltzer <mark.veltzer@gmail.com>

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

#include<firstinclude.h>
#include<stdlib.h> // for EXIT_SUCCESS
#include<string.h> // for strsignal(3)
#include<stdio.h> // for printf(3)
#include<signal.h> // for signal names

/*
* This is an example of using strsignal(3) to print out signal names.
* List of signals can be found in [man 7 signal]
* You can find them in [/usr/include/asm/signal.h]
* [kill -l] in bash (builtin) will list all signals.
* [kill -l] or [kill --list] from [/bin/kill] will list them too.
* [kill -L] or [kill --table] from [/bin/kill] will show them in a nice table.
*/

typedef struct _sig_and_string {
	int sig;
	const char* signame;
} sig_and_name;

sig_and_name sig_vals[]={
	{ SIGHUP, "SIGHUP" },
	{ SIGINT, "SIGINT" },
	{ SIGQUIT, "SIGQUIT" },
	{ SIGILL, "SIGILL" },
	{ SIGTRAP, "SIGTRAP" },
	{ SIGABRT, "SIGABRT" },
	{ SIGIOT, "SIGIOT" },
	{ SIGBUS, "SIGBUS" },
	{ SIGFPE, "SIGFPE" },
	{ SIGKILL, "SIGKILL" },
	{ SIGUSR1, "SIGUSR1" },
	{ SIGSEGV, "SIGSEGV" },
	{ SIGUSR2, "SIGUSR2" },
	{ SIGPIPE, "SIGPIPE" },
	{ SIGALRM, "SIGALRM" },
	{ SIGTERM, "SIGTERM" },
	{ SIGSTKFLT, "SIGSTKFLT" },
	{ SIGCHLD, "SIGCHLD" },
	{ SIGCONT, "SIGCONT" },
	{ SIGSTOP, "SIGSTOP" },
	{ SIGTSTP, "SIGTSTP" },
	{ SIGTTIN, "SIGTTIN" },
	{ SIGTTOU, "SIGTTOU" },
	{ SIGURG, "SIGURG" },
	{ SIGXCPU, "SIGXCPU" },
	{ SIGXFSZ, "SIGXFSZ" },
	{ SIGVTALRM, "SIGVTALRM" },
	{ SIGPROF, "SIGPROF" },
	{ SIGWINCH, "SIGWINCH" },
	{ SIGIO, "SIGIO" },
	{ SIGPOLL, "SIGPOLL" },
	{ SIGPWR, "SIGPWR" },
	{ SIGSYS, "SIGSYS" },
	{ SIGUNUSED, "SIGUNUSED" },
	{ SIGRTMIN, "SIGRTMIN" },
	{ SIGRTMAX, "SIGRTMAX" },
};

int main(int argc,char** argv,char** envp) {
	printf("number of signal values is %d\n",sizeof(sig_vals)/sizeof(sig_and_name));
	for(unsigned int i=0;i<sizeof(sig_vals)/sizeof(sig_and_name);i++) {
		int sig=sig_vals[i].sig;
		const char* signame=sig_vals[i].signame;
		printf("i=%d, sig=%d, in_code=%s, strsignal(sig)=%s\n",i,sig,signame,strsignal(sig));
	}
	return EXIT_SUCCESS;
}
