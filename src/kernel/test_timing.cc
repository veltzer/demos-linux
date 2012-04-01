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

#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/ioctl.h>

#include"shared.h"

#include<us_helper.h>

/*
 * A test that runs ioctl 0 with no data, sleeps for 1 second and then
 * another ioctl 1 with no data.
 */
int main(int argc, char **argv, char **envp) {
	// file to be used
	const char *filename = "/dev/mod_timing";
	printf("Inserting the driver...\n");
	my_system("sudo rmmod mod_timing");
	my_system("sudo insmod ./mod_timing.ko");
	my_system("sudo chmod 666 %s",filename);
	// file descriptor
	int d;

	printf("Starting\n");
	CHECK_NOT_M1(d = open(filename, O_RDWR));

	printf("showing cpus and their frequencies\n");
	klog_clear();
	CHECK_NOT_M1(ioctl(d, IOCTL_TIMING_CLOCK, NULL));
	klog_show_clear();
	// sleep for 1 second to allow us to see the results
	//sleep(1);

	klog_clear();
	CHECK_NOT_M1(ioctl(d, IOCTL_TIMING_TSC, 1000));
	klog_show_clear();

	klog_clear();
	CHECK_NOT_M1(ioctl(d, IOCTL_TIMING_JIFFIES, 1000));
	klog_show_clear();

	struct timeval t1, t2;
	const unsigned int loop=1000000;
	printf("doing %d syscalls\n",loop);
	gettimeofday(&t1, NULL);
	for (unsigned int i = 0;i < loop;i++) {
		CHECK_NOT_M1(ioctl(d, IOCTL_TIMING_EMPTY, NULL));
	}
	gettimeofday(&t2, NULL);
	printf("time in micro of one syscall: %lf\n", micro_diff(&t1,&t2)/(double)loop);

	CHECK_NOT_M1(close(d));
	return(0);
}
