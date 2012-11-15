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
#include<sys/types.h> // for kill(2)
#include<signal.h> // for kill(2)
#include<unistd.h> // for sleep(3)
#include<stdio.h> // for fprintf(3)
#include<stdlib.h> // for EXIT_SUCCESS, exit(3), EXIT_FAILURE, atoi(3)

int main(int argc,char** argv,char** envp) {
	if (argc < 2) {
		fprintf(stderr, "Usage: %s 1 or 2\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	int me=atoi(argv[1]);
	if (me < 0 || me > 2) {
		fprintf(stderr, "I said 1 or 2\n");
		exit(EXIT_FAILURE);
	}
	int sigme=0;
	switch(me) {
		case 1:
			sigme=SIGUSR1;
			break;
		case 2:
			sigme=SIGUSR2;
			break;
	}
	while(true) {
		sleep(2);
		kill(getppid(),sigme);
	}
	return EXIT_SUCCESS;
}
