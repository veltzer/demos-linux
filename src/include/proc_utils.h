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
#include <sys/types.h>	// for getpid(2)
#include <unistd.h>	// for getpid(2)
#include <stdio.h>	// for snprintf(3)
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

#endif	/* !__proc_utils_h */
