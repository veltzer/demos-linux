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

#ifndef __multiproc_utils_h
#define __multiproc_utils_h

/*
 * This is a collection of function to help with the linux
 * multi processing API
 */

/* THIS IS A C FILE, NO C++ here */

#include <firstinclude.h>
#include <us_helper.h>	// for TRACE()
#include <sys/types.h>	// for WIFSIGNALED(3), WTERMSIG(3), WIFEXITED(3), WEXITSTATUS(3)
#include <sys/wait.h>	// for WIFSIGNALED(3), WTERMSIG(3), WIFEXITED(3), WEXITSTATUS(3)
#include <string.h>	// for strsignal(3)
#include <stdio.h>	// for stderr, fprintf(3)

static inline int child_ok(int status) {
	if (WIFEXITED(status)) {
		if (WEXITSTATUS(status)) {
			return 0;
		} else {
			return 1;
		}
	} else {
		return 0;
	}
}

static inline void print_status(int status) {
	TRACE("analyzing code [%d]", status);
	if (WIFEXITED(status)) {
		TRACE("child exited normally with status %d", WEXITSTATUS(status));
	}
	if (WIFSTOPPED(status)) {
		TRACE("child was stopped with signal %d,%s", WSTOPSIG(status), strsignal(WSTOPSIG(status)));
	}
	if (WIFSIGNALED(status)) {
		TRACE("child was signaled with signal %d,%s", WTERMSIG(status), strsignal(WTERMSIG(status)));
	}
}

static inline void print_code(int code) {
	switch (code) {
	case CLD_EXITED:
		TRACE("child exited of it's own accord");
		break;

	case CLD_KILLED:
		TRACE("child was killed");
		break;

	case CLD_STOPPED:
		TRACE("child was stopped");
		break;

	case CLD_CONTINUED:
		TRACE("child was continued");
		break;
	}
}

/*
 * An enhanced system(3) version which also:
 * - accepts variable argument and does the substitution.
 * - checks for errors on return from system(3)
 */
static inline void my_system(const char *fmt, ...) {
	const unsigned int cmd_size=1024;
	char str[cmd_size];
	va_list args;

	va_start(args, fmt);
	vsnprintf(str, cmd_size, fmt, args);
	va_end(args);
#ifdef MULTIPROC_DEBUG
	fprintf(stderr, "doing [%s]\n", str);
#endif
	int ret=CHECK_NOT_M1(system(str));
	if(!child_ok(ret)) {
		CHECK_ASSERT(0);
	}
}

void my_system(const char *fmt, ...) __attribute__((format(printf, 1, 2)));

/*
 * Print the process name from /proc
 */
static inline void print_process_name_from_proc(void) {
	// my_system("cat /proc/%d/comm", getpid());
	my_system("cat /proc/self/comm");
}

/*
 * Print kernel memory buddy info from /proc
 */
static inline void printbuddy(void) {
	my_system("cat /proc/buddyinfo");
}

#endif	/* !__multiproc_utils_h */
