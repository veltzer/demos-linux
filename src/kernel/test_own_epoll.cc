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
#include <stdio.h>	// for fprintf(3), fflush(3)
#include <unistd.h>	// for sleep(3), close(2)
#include <pthread.h>	// for pthread_create(3), pthread_join(3)
#include <sys/types.h>	// for open(2)
#include <sys/stat.h>	// for open(2)
#include <fcntl.h>	// for open(2)
#include <sys/ioctl.h>	// for ioctl(2)
#include <err_utils.h>	// for CHECK_NOT_M1(), CHECK_ZERO_ERRNO()
#include <multiproc_utils.h>	// for my_system()
#include <trace_utils.h>	// for INFO()
#include "shared.h"	// for ioctl numbers

/*
 * EXTRA_LINK_FLAGS=-lpthread
 */

// file descriptors
static int fd;

static void *function_wait(void *p) {
	INFO("going to sleep...\n");
	CHECK_NOT_M1(ioctl(fd, IOCTL_EPOLL_WAIT, NULL));
	INFO("woke up...\n");
	return NULL;
}

static void *function_wakeup(void *p) {
	INFO("going to sleep...\n");
	sleep(3);
	INFO("waking up...\n");
	CHECK_NOT_M1(ioctl(fd, IOCTL_EPOLL_WAKE, NULL));
	INFO("finished waking up...\n");
	return NULL;
}

int main(int argc, char** argv, char** envp) {
	const char* basename="own_epoll";
	char filename[256];
	snprintf(filename, 256, "/dev/mod_%s", basename);
	printf("Inserting the driver...\n");
	my_system("sudo rmmod mod_%s", basename);
	my_system("sudo insmod ./mod_%s.ko", basename);
	my_system("sudo chmod 666 %s", filename);

	fd=CHECK_NOT_M1(open(filename, O_RDWR));

	pthread_t thread_wait, thread_wakeup;
	CHECK_ZERO_ERRNO(pthread_create(&thread_wait, NULL, function_wait, NULL));
	CHECK_ZERO_ERRNO(pthread_create(&thread_wakeup, NULL, function_wakeup, NULL));
	CHECK_ZERO_ERRNO(pthread_join(thread_wait, NULL));
	CHECK_ZERO_ERRNO(pthread_join(thread_wakeup, NULL));
	CHECK_NOT_M1(close(fd));
	fprintf(stdout, "\nALL DONE\n");
	return(0);
}
