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

#ifndef __multi_processing_h
#define __multi_processing_h

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

#endif	/* !__multi_processing_h */
