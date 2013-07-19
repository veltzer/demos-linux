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

#ifndef __process_utils_h
#define __process_utils_h

/*
 * This is a collection of utils to help you handle processes.
 */

/* THIS IS A C FILE, NO C++ here */

#include <firstinclude.h>
#include <sys/prctl.h>	// for prctl(2), PR_SET_NAME, PR_GET_NAME
#include <string.h>	// for strncpy(3)
#include <err_utils.h>	// for CHECK_ZERO()
#include <trace_utils.h>// for TRACE()

const unsigned int name_size=16;
/*
 * Print the current processes name
 */
static inline void process_print_name(void) {
	char name[name_size];
	CHECK_ZERO(prctl(PR_GET_NAME, name));
	TRACE("process name is [%s]", name);
}

/*
 * Get the current processes name
 */
static inline void process_get_name(char* buffer, unsigned int bufsize) {
	char name[name_size];
	CHECK_ZERO(prctl(PR_GET_NAME, name));
	strncpy(buffer, name, sizeof(name));
}

/*
 * Set the current processes(thread) name
 */
static inline void process_set_name(const char* newname) {
	char name[name_size];
	strncpy(name, newname, name_size);
	CHECK_ZERO(prctl(PR_SET_NAME, name));
}

#endif	/* !__process_utils_h */
