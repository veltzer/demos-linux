/*
 * This file is part of the linuxapi package.
 * Copyright (C) 2011-2013 Mark Veltzer <mark.veltzer@gmail.com>
 *
 * linuxapi is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * linuxapi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with linuxapi. If not, see <http://www.gnu.org/licenses/>.
 */

#include <firstinclude.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <us_helper.h>	// for CHECK_NOT_M1()
#include "shared.h"

/*
 * A test that runs ioctl 0 with no data, sleeps for 1 second and then
 * another ioctl 1 with no data.
 */
int main(int argc, char** argv, char** envp) {
	// file to be used
	const char *filename="/dev/mod_timing";
	printf("Inserting the driver...\n");
	my_system("sudo rmmod mod_timing");
	my_system("sudo insmod ./mod_timing.ko");
	my_system("sudo chmod 666 %s", filename);

	printf("Starting\n");
	int d=CHECK_NOT_M1(open(filename, O_RDWR));

	printf("showing cpus and their frequencies\n");
	klog_clear();
	CHECK_NOT_M1(ioctl(d, IOCTL_TIMING_CLOCK, NULL));
	klog_show_clear();
	// sleep for 1 second to allow us to see the results
	// CHECK_ZERO(sleep(1));

	klog_clear();
	CHECK_NOT_M1(ioctl(d, IOCTL_TIMING_TSC, 1000));
	klog_show_clear();

	klog_clear();
	CHECK_NOT_M1(ioctl(d, IOCTL_TIMING_JIFFIES, 1000));
	klog_show_clear();

	struct timeval t1, t2;
	const unsigned int loop=1000000;
	printf("doing %d syscalls\n", loop);
	gettimeofday(&t1, NULL);
	for (unsigned int i=0; i < loop; i++) {
		CHECK_NOT_M1(ioctl(d, IOCTL_TIMING_EMPTY, NULL));
	}
	gettimeofday(&t2, NULL);
	printf("time in micro of one syscall: %lf\n", micro_diff(&t1, &t2)/(double)loop);

	CHECK_NOT_M1(close(d));
	return(0);
}
