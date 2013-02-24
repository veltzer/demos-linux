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
#include <stdio.h>	// for printf(3)
#include <sched.h>	// for sched_getcpu(2)
#include <unistd.h>	// for sleep(3)
#include <sys/types.h>	// for getpid(2)
#include <unistd.h>	// for getpid(2)
#include <stdlib.h>	// for EXIT_SUCCESS

/*
 * This is an example of a process that prints the CPU it is running on
 * and lets you change the CPU that it is on.
 */

int main(int argc, char** argv, char** envp) {
	pid_t pid=getpid();
	printf("change my cpu using [taskset --cpu-list --pid [list] %d]\n", pid);
	int cpunum=sched_getcpu();
	printf("sched_getcpu() says I'm running on core %d\n", cpunum);
	while(true) {
		int newcpunum=sched_getcpu();
		if(cpunum!=newcpunum) {
			cpunum=newcpunum;
			printf("I've switched to running on core %d\n", cpunum);
		}
		sleep(3);
	}
	return EXIT_SUCCESS;
}
