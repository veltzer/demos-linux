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

#include<curses.h>
#include<stdlib.h> // for EXIT_SUCCESS

/*
 * Curses example program number 2...
 *
 * This is an example of how to read unbufferd input from stdin...
 * In this example we want to read JUST A SINGLE character...
 *
 * This example uses curses to achieve this goal although other
 * tools may be used to achieve the same goal
 * (system calls for instance).
 *
 * EXTRA_LIBS=-lncursesw
 */
int main(int argc,char** argv,char** envp) {
	initscr();
	cbreak();
	noecho();
	return EXIT_SUCCESS;
}
