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
#include <stdio.h>	// for stdin, fileno(3)
#include <unistd.h>	// for isatty(3), ttyname(3)
#include <stdlib.h>	// for EXIT_SUCCESS
#include <trace_utils.h>// for TRACE()

/*
 * This is an example of how to detect whether a file you are using is
 * a terminal or not and print it's name in case it is.
 * This is similar to the tty(1) command line utility.
 */

int main(int argc, char** argv, char** envp) {
	// lets take the file descriptor number from stdin which is usually
	// a terminal (unless you redirect it...)
	int filedes=fileno(stdin);
	if (isatty(filedes)) {
		TRACE("it is a terminal with name [%s]", ttyname(filedes));
	} else {
		TRACE("it is not a terminal");
	}
	return EXIT_SUCCESS;
}
