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

#include<unistd.h> // for nice(2), fork(2)
#include<sys/types.h> // for getpid(2)
#include<unistd.h> // for getpid(2)
#include<sched.h> // for sched_setaffinity(2), CPU_ZERO(3), CPU_SET(3)

#include<us_helper.h> // for CHECK_ZERO, CHECK_NOT_M1, TRACE

/*
 * This examples shows the effect of the nice scheduling system.
 *
 * It binds the process to a single core (for multi core systems).
 * It then forks two versions of heavy work each with a different nice level.
 * Then you can compare the work done by each of these.
 *
 * References:
 * man 2 nice
 *
 * TODO:
 * - make the printing look nicer so that it would be obvious that the child
 * and the parent are running in different speeds.
 */

int main(int argc,char** argv,char** envp) {
	// bind the entire process to a single CPU
	// lets get our pid (no error for getpid)
	pid_t mypid=getpid();
	cpu_set_t onecore;
	CPU_ZERO(&onecore);
	CPU_SET(0,&onecore);
	CHECK_NOT_M1(sched_setaffinity(mypid,sizeof(onecore),&onecore));
	// lets fork...
	pid_t pid;
	CHECK_NOT_M1(pid=fork());
	if(pid==0) {
		// child
		CHECK_NOT_M1(nice(5));
	}
	float sum;
	for(unsigned int i=0;i<100000000;i++) {
		for(unsigned int j=0;j<100000000;j++) {
			sum+=j;
		}
		TRACE("i is %d",i);
	}
	return EXIT_SUCCESS;
}
