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

#ifndef __multi_processing_h
#define __multi_processing_h

#include <firstinclude.h>
#include <us_helper.h>	// for TRACE()
#include <sys/types.h>	// for WIFSIGNALED(3), WTERMSIG(3), WIFEXITED(3), WEXITSTATUS(3)
#include <sys/wait.h>	// for WIFSIGNALED(3), WTERMSIG(3), WIFEXITED(3), WEXITSTATUS(3)
#include <string.h>	// for strsignal(3)

/* THIS IS A C FILE, NO C++ here */

static inline void print_status(int status) {
	TRACE("analyzing code [%d]\n", status);
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
