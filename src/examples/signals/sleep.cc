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
#include<unistd.h> // for sleep(3)
#include<stdlib.h> // for EXIT_SUCCESS, EXIT_FAILURE, atoi(3)
#include<us_helper.h> // for CHECK_ZERO()

/*
* This is a simple example of calling sleep(3).
* The idea is to demonstrate that sleep is made out of signal
* handling functions. strace this executable to see exactly which.
*
* Here is the output:
* rt_sigprocmask(SIG_BLOCK, [CHLD], [], 8) = 0
* rt_sigaction(SIGCHLD, NULL, {SIG_DFL, [], 0}, 8) = 0
* rt_sigprocmask(SIG_SETMASK, [], NULL, 8) = 0
* nanosleep({5, 0}, 0xbf9db174) = 0
*/

int main(int argc,char** argv,char** envp) {
	if(argc!=2) {
		fprintf(stderr,"usage: %s [seconds]\n",argv[0]);
		return EXIT_FAILURE;
	}
	int seconds=atoi(argv[1]);
	CHECK_ZERO(sleep(seconds));
	return EXIT_SUCCESS;
}
