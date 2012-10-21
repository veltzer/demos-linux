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
#include<iostream> // for std::cerr
#include<signal.h>
#include<unistd.h> // for pipe(2)
#include<stdio.h> // for perror(3)
#include<errno.h> // for perror(3)
#include<stdlib.h> // for EXIT_SUCCESS

/*
* This example shows how to eliminate lots of redundant C error checking
* from your code when calling system calls or C APIs from C++ and turning
* all of those errors into exceptions...
*
* Why is this good ? Explain the theory behind exceptions.
*/

// this is the non template approach
int syscall(int val,int err_val) {
	if(val==err_val) {
		//perror("C++ exception thrown");
		throw std::exception();
	}
	return val;
}

// Here is a template that will take care of all our needs
template<class T> T syscall(T val, T err_val) {
	if(val==err_val) {
		//perror("C++ exception thrown");
		throw std::exception();
	}
	return(val);
}


void myhandler(int sig) {
	std::cerr << "in signal handler for signal " << sig << std::endl;
}

int main(int argc,char** argv,char** envp) {
	// here is an example of using this construct
	int fd[2];
	try {
		syscall(pipe(fd), -1);
		syscall(signal(SIGPIPE, myhandler), SIG_ERR);
	} catch (std::exception e) {
		std::cerr << "cought exception" << std::endl;
	}
	return EXIT_SUCCESS;
}
