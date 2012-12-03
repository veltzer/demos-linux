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
#include<unistd.h> // for daemon(3), pause(2), getpid(2), getppid(2)
#include<stdlib.h> // for EXIT_SUCCESS
#include<stdio.h> // for printf(3)
#include<us_helper.h> // for CHECK_NOT_M1()

/*
* This is an example of using the daemon(3) function.
*/

int main(int argc,char** argv,char** envp) {
	// this print is still shown on the terminal...
	printf("before daemon(3) ,I am id [%d], my parent is [%d]\n",getpid(),getppid());
	CHECK_NOT_M1(daemon(0,0));
	// TODO: this print does not get anywhere (/dev/null redirection).
	// use syslog instead.
	printf("after daemon(3) ,I am id [%d], my parent is [%d]\n",
			getpid(),getppid());
	while(true) {
		pause();
	}
	return EXIT_SUCCESS;
}
