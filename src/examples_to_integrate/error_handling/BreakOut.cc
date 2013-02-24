/*
        This file is part of the linuxapi project.
        Copyright (C) 2011-2013 Mark Veltzer <mark.veltzer@gmail.com>

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
#include <iostream>	// for std::cerr, std::endl
#include <signal.h>	// for signal(2), siginterrupt(3)
#include <unistd.h>	// for read(2)
#include <stdlib.h>	// for EXIT_SUCCESS, EXIT_FAILURE, exit(3)
#include <string.h>	// for strsignal(3)
#include <us_helper.h>	// for CHECK_NOT_M1(), CHECK_NOT_SIGT()

static int counterUSR1=0;
static int counterUSR2=0;
static int flag=0;

static void SignalHandlerUSR1(int sig) {
	counterUSR1++;
	std::cerr << "handler: [" << strsignal(sig) << "]: " << counterUSR1 << " starting" << std::endl;
}

static void SignalHandlerUSR2(int sig) {
	counterUSR2++;
	std::cerr << "handler: [" << strsignal(sig) << "]: " << counterUSR1 << " starting" << std::endl;
	// reverse the flag
	if(flag==0) {
		flag=1;
	} else {
		flag=0;
	}
	std::cerr << "handler: [" << strsignal(sig) << "]: " << counterUSR1 << " setting flag to " << flag << std::endl;
	CHECK_NOT_M1(siginterrupt(SIGUSR1, flag));
}

int main(int argc, char** argv, char** envp) {
	// set up the signal handler (only need to do this once)
	CHECK_NOT_SIGT(signal(SIGUSR1, SignalHandlerUSR1), SIG_ERR);
	CHECK_NOT_SIGT(signal(SIGUSR2, SignalHandlerUSR2), SIG_ERR);
	std::cerr << "main: set up the sig handler, lets start" << std::endl;
	// This is a non busy wait loop which only wakes up when there
	// are signals
	const int size=256;
	char buf[size];
	while(true) {
		std::cerr << "main: before read" << std::endl;
		CHECK_NOT_M1(read(0, buf, size));
		std::cerr << "main: after read" << std::endl;
	}
	return EXIT_SUCCESS;
}
