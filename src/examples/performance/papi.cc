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
#include <papi.h>	// for PAPI_* functions
#include <unistd.h>	// for usleep(3)
#include <stdio.h>	// for printf(3), fprintf(3)
#include <stdlib.h>	// for atoi(3), EXIT_FAILURE, EXIT_SUCCESS
#include <us_helper.h>	// for getticks(), CHECK_INT(), CHECK_NOT_M1()

/*
 * Demo for using the PAPI library for RDTSC.
 * This example also proves that RDTSC is exactly what the PAPI library
 * is using under the hood.
 *
 * References:
 * http://icl.cs.utk.edu/papi/index.html
 *
 * EXTRA_LINK_FLAGS=-lpapi
 */

int main(int argc, char** argv, char** envp) {
	if(argc!=2) {
		fprintf(stderr, "%s: usage: %s [usecs]\n", argv[0], argv[0]);
		fprintf(stderr, "%s: example is 1000000 which means 1 second\n", argv[0]);
		return EXIT_FAILURE;
	}
	unsigned int usecs=atoi(argv[1]);

	// init papi
	CHECK_INT(PAPI_library_init(PAPI_VER_CURRENT), PAPI_VER_CURRENT);

	// lets start measuring...
	long long c1=PAPI_get_real_cyc();
	ticks_t t1=getticks();
	CHECK_NOT_M1(usleep(usecs));
	long long c2=PAPI_get_real_cyc();
	ticks_t t2=getticks();
	printf("c1(papi) is %lld\n", c1);
	printf("c2(papi) is %lld\n", c2);
	printf("diff (papi): %lld\n", c2-c1);
	printf("t1(tsc) is %lld\n", t1);
	printf("t2(tsc) is %lld\n", t2);
	printf("diff (tsc): %lld\n", t2-t1);
	return EXIT_SUCCESS;
}
