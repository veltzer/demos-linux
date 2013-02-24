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
#include <iostream>	// for std::cerr, std::endl, std::exception
#include <signal.h>	// for signal(2)
#include <stdlib.h>	// for EXIT_SUCCESS
#include <unistd.h>	// for pipe(2)

// Here is a template that will take care of all out needs

/*
   int syscall(int val) {
        if(val==-1) {
                throw std::exception();
        }
        return val;
   }
 */

template<class T> T mysyscall(T val, T err_val) {
	if(val==err_val) {
		throw std::exception();
	}
	return val;
}

void myhandler(int sig) {
	std::cerr << "in signal handler for signal " << sig << std::endl;
}

int main(int argc, char** argv, char** envp) {
	// here is an example of using this construct
	int fd[2];
	try {
		mysyscall(pipe(fd), -1);
		mysyscall(signal(SIGPIPE, myhandler), SIG_ERR);
	}catch(std::exception e) {
		std::cerr << "cought exception" << std::endl;
	}
	return EXIT_SUCCESS;
}
