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

/*
 * This is the process which is monitored by the watchdog in the watchdog
 * exercise.
 * It just sleeps for a variable amount of time and the dies with a signal.
 */

#include <firstinclude.h>
#include <stdlib.h>	// for EXIT_SUCCCESS, abort(3)
#include <us_helper.h>	// for CHECK_NOT_M1()
#include <stdio.h>	// for printf(3)

int main(int argc, char** argv, char** envp) {
	const unsigned int min_sleep=5;
	const unsigned int max_sleep=10;
	int sleep_time=min_sleep+rand()%(max_sleep-min_sleep);
	printf("child: sleeping for %d seconds\n", sleep_time);
	CHECK_ZERO(sleep(sleep_time));
	return EXIT_SUCCESS;
}