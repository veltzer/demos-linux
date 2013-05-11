/*
 * This file is part of the linuxapi project.
 * Copyright (C) 2011-2013 Mark Veltzer <mark.veltzer@gmail.com>
 *
 * The linuxapi package is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * The linuxapi package is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with the GNU C Library; if not, write to the Free
 * Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 * 02111-1307 USA.
 */

#include <firstinclude.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h> // for usleep(3)
#include <us_helper.h> // for CHECK_NOT_M1()

/*
 * This example shows how to use gettimeofday
 * This example also shows how accurate this method of measurement is...
 */
void print_timeval(struct timeval *tv, const char *name) {
	printf("%s - sec is %lu\n", name, tv->tv_sec);
	printf("%s - usec is %lu\n", name, tv->tv_usec);
}

unsigned long diff_timeval_in_micro(struct timeval *tv1, struct timeval *tv2) {
	if (tv1->tv_sec > tv2->tv_sec) {
		fprintf(stderr, "tv1>tv2\n");
		exit(-1);
	} else {
		if (tv1->tv_sec==tv2->tv_sec) {
			if (tv1->tv_usec > tv2->tv_usec) {
				fprintf(stderr, "tv1>tv2\n");
				exit(-1);
			}
		}
	}
	unsigned long diff=(tv2->tv_sec - tv1->tv_sec) * 1000;
	diff+=(tv2->tv_usec - tv1->tv_usec) / 1000;
	return(diff);
}

int main(int argc, char** argv, char** envp) {
	struct timeval tv_start, tv_end;

	CHECK_NOT_M1(gettimeofday(&tv_start, NULL));
	CHECK_NOT_M1(usleep(50 * 1000));
	CHECK_NOT_M1(gettimeofday(&tv_end, NULL));
	print_timeval(&tv_start, "start");
	print_timeval(&tv_end, "end");
	unsigned long diff=diff_timeval_in_micro(&tv_start, &tv_end);
	printf("diff is %lu\n", diff);
	return EXIT_SUCCESS;
}
