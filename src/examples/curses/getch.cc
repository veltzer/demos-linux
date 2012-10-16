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

#include<firstinclude.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<curses.h>
#include<unistd.h>

/*
* Curses example number 1...
*
* This is an example of how to read a character from the keyboard
* in non-blocking mode...
*
* EXTRA_LIBS=-lncursesw
*/

int main(int argc,char** argv,char** envp) {
	initscr();
	cbreak();
	noecho();
	while(true) {
		int c=getch();
		if (c==ERR) {
			// there is no input on the keyboard. Wait a while to avoid busy wait...
			//usleep(100000);
		} else {
			// We have a character!!! Print it on the screen...
			echochar(c);
			//printf("char is %d\n",c);
		}
	}
	endwin();
	return EXIT_SUCCESS;
}
