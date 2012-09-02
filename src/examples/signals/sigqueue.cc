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
#include<signal.h> // for sigqueue(2), SIGRTMIN, SIGRTMIN
#include<stdio.h> // for fprintf(3)
#include<stdlib.h> // for EXIT_SUCCESS, EXIT_FAILURE, atoi(3)
#include<us_helper.h> // for CHECK_NOT_M1()

/*
* This is a general command line utility to send signals via
* the sigqueue(2) interface.
*/

int main(int argc,char** argv,char** envp) {
	if (argc != 4) {
		fprintf(stderr,"usage: %s [pid] [sig] [value]\n",argv[0]);
		fprintf(stderr,"Info for real time signals follows:\n");
		fprintf(stderr,"SIGRTMIN is %d\n",SIGRTMIN);
		fprintf(stderr,"SIGRTMAX is %d\n",SIGRTMAX);
		return EXIT_FAILURE;
	}
	int pid=atoi(argv[1]);
	int sig=atoi(argv[2]);
	int val=atoi(argv[3]);
	union sigval sval;
	sval.sival_int=val;
	CHECK_NOT_M1(sigqueue(pid,sig,sval));
	return EXIT_SUCCESS;
}
