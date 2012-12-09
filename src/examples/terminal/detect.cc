/*
	This file is part of the linuxapi project.
	Copyright (C) 2011, 2012 Mark Veltzer <mark.veltzer@gmail.com>

	The linuxapi package is free software; you can redistribute it and/or
	modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 2.1 of the License, or (at your option) any later version.

	The linuxapi package is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public
	License along with the GNU C Library; if not, write to the Free
	Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
	02111-1307 USA.
*/

#include <firstinclude.h>
#include <stdio.h> // for stdin, fileno(3)
#include <unistd.h> // for isatty(3), ttyname(3)
#include <stdlib.h> // for EXIT_SUCCESS
#include <us_helper.h> // for DEBUG()

/*
* This is an example of how to detect whether a file you are using is
* a terminal or not and print it's name in case it is.
*/

int main(int argc,char** argv,char** envp) {
	// lets take the file descriptor number from stdin which is usually
	// a terminal (unless you redirect it...)
	int filedes=fileno(stdin);

	if (isatty(filedes)) {
		DEBUG("it is a terminal with name [%s]", ttyname(filedes));
	} else {
		DEBUG("it is not a terminal");
	}
	return EXIT_SUCCESS;
}
