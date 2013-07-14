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

#ifndef __proc_utils_h
#define __proc_utils_h

/*
 * This is a collection of helper function to help with dealing
 * with the Linux signal API
 */

/* THIS IS A C FILE, NO C++ here */

#include <firstinclude.h>
#include <sys/types.h>	// for getpid(2), pid_t
#include <unistd.h>	// for getpid(2), pid_t
#include <stdio.h>	// for snprintf(3)
#include <proc/readproc.h>	// for get_proc_stats(3), proc_t, look_up_our_self(3)
#include <multiproc_utils.h>	// for my_system()

/*
 * Function to print the current processes /proc maps file
 */
static inline void proc_print_mmap(const char *filter) {
	pid_t pid=getpid();
	if (filter==NULL) {
		my_system("cat /proc/%d/maps", pid);
	} else {
		my_system("cat /proc/%d/maps | grep %s", pid, filter);
	}
}

/*
 * Print memory stats for the current process
 */
static inline void proc_print_mem_stats(pid_t pid) {
	proc_t myproc;
	get_proc_stats(pid, &myproc);
	printf("size is %ld, min_flt is %ld, state is %c\n", myproc.rss, myproc.min_flt, myproc.state);
}

/*
 * Print memory stats for the current process
 */
static inline void proc_print_mem_stats_self(void) {
	proc_t myproc;
	look_up_our_self(&myproc);
	printf("size is %ld, min_flt is %ld\n", myproc.rss, myproc.min_flt);
}

/*
 * Print the name of any process according to pid
 */
void my_print_process_name_from_proc(pid_t pid) {
	my_system("cat /proc/%d/comm", pid);
}

/*
 * Print the name of our process via /proc
 */
void my_print_process_name_from_proc_self() {
	my_system("cat /proc/self/comm");
}

#endif	/* !__proc_utils_h */
