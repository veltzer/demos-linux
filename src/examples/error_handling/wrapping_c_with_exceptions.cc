#include<iostream> // for std::cerr
#include<signal.h>
#include<stdio.h> // for perror(3)
#include<errno.h> // for perror(3)
#include<stdlib.h> // for EXIT_SUCCESS

/*
 * This example shows how to eliminate lots of redundant C error checking
 * from your code when calling system calls or C APIs from C++ and turning
 * all of those errors into exceptions...
 *
 * Why is this good ? Explain the theory behind exceptions.
 *
 * 			Mark Veltzer
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

int main(int argc, char **argv, char **envp) {
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
