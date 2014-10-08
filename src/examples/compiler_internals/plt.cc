/*
 * This file is part of the linuxapi package.
 * Copyright (C) 2011-2014 Mark Veltzer <mark.veltzer@gmail.com>
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
#include <stdio.h>	// for printf(3)
#include <stdlib.h>	// for EXIT_SUCCESS
#include <dlfcn.h>	// for dlsym(3)
#include <sys/types.h>	// for getpid(2)
#include <unistd.h>	// for getpid(2)
#include <measure.h>	// for measure, measure_init(), measure_start(), measure_end(), measure_print()
#include <sched_utils.h>// for sched_run_priority(), SCHED_FIFO_HIGH_PRIORITY:const

/*
 * this example shows the difference between obtaining a real pointer to a function
 * via dlsym(3) and calling that function through the PLT.
 * - When you program "regularly" you call the function through the PLT and you can
 * see all the functions that you are using huddled closely in that table and having
 * constant addresses from run to run, unaffected by ASLR.
 * - when you call dlsym(3) you get the direct point to the function which, because
 * of ASLR which causes the standard C library to be loaded at a different address
 * on each run, will return a slightly different pointer on each invocation.
 * - so is using dlsym(3) for every function more efficient? well, yes but by very
 * little so you don't really want to do that. see the measurements that we show.
 *
 * EXTRA_LINK_FLAGS=-ldl -lpthread
 */

const unsigned int count=10000000;

// this one does straight getpid(2)
void* m1(void*) {
	measure m;
	// think! why count+1?
	measure_init(&m, "direct getpid", count+1);
	measure_start(&m);
	for(unsigned int i=0; i<count; i++) {
		getpid();
	}
	measure_end(&m);
	measure_print(&m);
	return NULL;
}

// this one does dlsym getpid(2)
void* m2(void*) {
	typeof(getpid)* p_dlsym_getpid=(typeof(getpid)*)dlsym(RTLD_NEXT, "getpid");
	measure m;
	// think! why count+1?
	measure_init(&m, "dlsym getpid", count+1);
	measure_start(&m);
	for(unsigned int i=0; i<count; i++) {
		p_dlsym_getpid();
	}
	measure_end(&m);
	measure_print(&m);
	return NULL;
}

int main(int argc, char** argv, char** envp) {
	typeof(atoi)* p_direct_atoi=&atoi;
	typeof(atoi)* p_dlsym_atoi=(typeof(atoi)*)dlsym(RTLD_NEXT, "atoi");
	typeof(printf)* p_direct_printf=&printf;
	typeof(printf)* p_dlsym_printf=(typeof(printf)*)dlsym(RTLD_NEXT, "printf");
	typeof(getpid)* p_direct_getpid=&getpid;
	typeof(getpid)* p_dlsym_getpid=(typeof(getpid)*)dlsym(RTLD_NEXT, "getpid");
	printf("p_direct_atoi is %p\n", p_direct_atoi);
	printf("p_dlsym_atoi is %p\n", p_dlsym_atoi);
	printf("p_direct_printf is %p\n", p_direct_printf);
	printf("p_dlsym_printf is %p\n", p_dlsym_printf);
	printf("p_direct_getpid is %p\n", p_direct_getpid);
	printf("p_dlsym_getpid is %p\n", p_dlsym_getpid);
	printf("p_direct_atoi(2) is %d\n", p_direct_atoi("2"));
	printf("p_dlsym_atoi(2) is %d\n", p_dlsym_atoi("2"));
	sched_run_priority(m2, NULL, SCHED_FIFO_HIGH_PRIORITY, SCHED_FIFO);
	sched_run_priority(m1, NULL, SCHED_FIFO_HIGH_PRIORITY, SCHED_FIFO);
	return EXIT_SUCCESS;
}
