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
#include<sched.h> // for CPU_COUNT(3), CPU_SETSIZE, CPU_ISSET(3)
#include<unistd.h> // for sysconf(3)

#include<us_helper.h> // for TRACE(), CHECK_ZERO()

/*
* This is a demo of how to use a cpu set
*
* TODO:
* - move the print_cpu_set function into the shared code. find other uses
* of it and eliminate them.
*/
void print_cpu_set(cpu_set_t *p) {
	TRACE("CPU_COUNT is %d", CPU_COUNT(p));
	TRACE("CPU_SETSIZE is %d", CPU_SETSIZE);
	for(int j=0;j<CPU_SETSIZE;j++) {
		if (CPU_ISSET(j, p)) {
			TRACE("\tCPU %d", j);
		}
	}
}

int main(int argc,char** argv,char** envp) {
	TRACE("number of cpus is %ld", sysconf(_SC_NPROCESSORS_ONLN));
	cpu_set_t cpuset;
	CPU_ZERO(&cpuset);
	for(int i=0;i<500;i+=100) {
		CPU_SET(i,&cpuset);
	}
	print_cpu_set(&cpuset);
	return EXIT_SUCCESS;
}
