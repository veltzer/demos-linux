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
#include <stdlib.h> // for EXIT_SUCCESS
#include <time.h> // for clock_gettime(2), clock_nanosleep(2)
#include <sched.h> // for sched_setscheduler(2), struct sched_param
#include <sys/mman.h> // for mlockall(2)
#include <string.h> // for memset(3)
#include <us_helper.h> // for CHECK_NOT_M1()

/*
* This is a real time skeleton that shows all the critical parts of a real time application
* in Linux
*
* EXTRA_LINK_FLAGS=-lrt
*/

/* we use 49 as the PRREMPT_RT use 50
as the priority of kernel tasklets
and interrupt handler by default */
const int MY_PRIORITY=49;
/* The maximum stack size which is
guranteed safe to access without
faulting */
const int MAX_SAFE_STACK=8*1024;
/* The number of nsecs per sec. */
const int NSEC_PER_SEC=1000000000;

void stack_prefault(void) {
	unsigned char dummy[MAX_SAFE_STACK];
	memset(&dummy, 0, MAX_SAFE_STACK);
}

int main(int argc,char** argv,char** envp) {
	/* Declare ourself as a real time task */
	struct sched_param param;
	param.sched_priority=49;
	CHECK_NOT_M1(sched_setscheduler(0, SCHED_FIFO, &param));
	/* Lock memory */
	CHECK_NOT_M1(mlockall(MCL_CURRENT|MCL_FUTURE));
	/* Pre-fault our stack */
	stack_prefault();

	// get the current time
	struct timespec t;
	clock_gettime(CLOCK_MONOTONIC,&t);
	/* start after one second */
	t.tv_sec++;

	const int interval=50000; /* 50us*/
	while(true) {
		/* wait untill next shot */
		clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &t, NULL);
		/* do the stuff */
		/* ... */
		/* calculate next shot */
		t.tv_nsec+=interval;
		while(t.tv_nsec>=NSEC_PER_SEC) {
			t.tv_nsec-=NSEC_PER_SEC;
			t.tv_sec++;
		}
	}
	return EXIT_SUCCESS;
}
