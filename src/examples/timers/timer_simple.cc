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

#include <firstinclude.h>
#include <pthread.h> // for pthread_t, pthread_create(3), pthread_join(3), pthread_self(3)
#include <stdlib.h> // for EXIT_SUCCESS
#include <signal.h> // for timer_create(2), signal(2)
#include <time.h> // for timer_create(2), timer_settime(2), timer_delete(2)
#include <unistd.h> // for pause(2)
#include <us_helper.h> // for CHECK_NOT_M1(), CHECK_NOT_SIGT()

/*
* This is a standard pthread demo
*
* EXTRA_LINK_FLAGS=-lrt
*/
void sigusr(int signal) {
	return;
}

int main(int argc,char** argv,char** envp) {
	timer_t timerid;
	struct itimerspec tick={{0,10*1000},{0,10*1000}};
	struct sigevent sigev;
	sigev.sigev_notify=SIGEV_SIGNAL;
	sigev.sigev_signo=SIGUSR1;
	CHECK_NOT_SIGT(signal(SIGUSR1,sigusr),SIG_ERR);
	CHECK_NOT_M1(timer_create(CLOCK_MONOTONIC,&sigev,&timerid));
	CHECK_NOT_M1(timer_settime(timerid,0,&tick,NULL));
	while(true) {
		pause();
	}
	CHECK_NOT_M1(timer_delete(timerid));
	return EXIT_SUCCESS;
}
