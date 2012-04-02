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
#include<stdio.h> // for printf(3)
#include<unistd.h> // for write(2), _exit(2)
#include<stdlib.h> // for EXIT_SUCCESS, EXIT_FAILURE

int func(int a, int b) {
	return(a + b);
}

/*
void do_init(void) __attribute__((constructor));
void do_init(void) {
	printf("Hello\n");
}
*/

int my_entry(int argc,char** argv,char** envp) {
	/*
	_exit(EXIT_FAILURE);
	int ret=write(1,"hello\n",6);
	if(ret!=6) {
		_exit(EXIT_FAILURE);
	}
	//printf("hello from the lib...");
	*/
	return EXIT_SUCCESS;
}
