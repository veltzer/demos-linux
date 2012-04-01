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

#include<iostream>
#include<signal.h>
#include<stdlib.h> // for exit(3), EXIT_SUCCESS, EXIT_FAILURE

static void printStatus() {
	sigset_t old;
	if(sigprocmask(SIG_BLOCK,NULL,&old)==-1) {
		perror("problem with calling sigprocmask(2)");
		exit(EXIT_FAILURE);
	}
	int ret=sigismember(&old,SIGFPE);
	if(ret==-1) {
		perror("problem with calling sigismember(2)");
		exit(EXIT_FAILURE);
	}
	std::cerr << "ret is " << ret << std::endl;
}

/*
 * This method unblocks a signal so that it could be gotten again.
 * The important thing to understand is that in a signal handler for signal X
 * the same signal is blocked. This means that since we throw an exception from
 * a signal handler (which actually does a longjmp) then this situation will not
 * change and so we need to unblock the signal if we want to use C++ exception handling
 * or longjmp
 */
static void unblock(int signum) {
	sigset_t sigs;
	sigemptyset(&sigs);
	sigaddset(&sigs,signum);
	if(sigprocmask(SIG_UNBLOCK,&sigs,NULL)==-1) {
		perror("problem with calling sigprocmask(2)");
		exit(EXIT_FAILURE);
	}
}

static void SignalHandler(int sig) {
	// before we throw an exception or do a longjmp we need to unblock
	// the signal or the kernel will think we are still in the signal handler
	unblock(sig);
	throw std::exception();
}

static void doBadCode(int i) {
	int x,y;
	char* p;
	switch(i) {
		/*
		 * This is a standard FPE exception
		 */
		case 0:
			std::cerr << "before division by zero" << std::endl;
			// turning x into float here would not work since it
			// will not cause an exception
			x=0;
			y=1/x;
			std::cerr << "y is " << y << std::endl;
			std::cerr << "this is never reached" << std::endl;
			break;
		/*
		 * This is a segmentation fault using a straight illegal memory access
		 */
		case 1:
			std::cerr << "Lets access some illegal memory address" << std::endl;
			p=0;
			p[0]=0;
			break;
		/*
		 * This is a floating point exception by using raise(3)
		 */
		case 2:
			std::cerr << "Lets do a simulation of some bad code using raise(3)" << std::endl;
			if(raise(SIGFPE)==-1) {
				perror("problem with calling raise(2)");
				exit(EXIT_FAILURE);
			}
			break;
		/*
		 * This is a floating point exception by using kill(2)
		 */
		case 3:
			std::cerr << "Lets do a simulation of some bad code using kill(2)" << std::endl;
			if(kill(getpid(),SIGFPE)==-1) {
				perror("problem with calling kill(2)");
				exit(EXIT_FAILURE);
			}
			break;
	}
}

int main(int argc,char** argv,char** envp) {
	// set up the signal handler (only need to do this once)
	if(signal(SIGFPE,SignalHandler)==SIG_ERR) {
		perror("problem with calling signal(2)");
		exit(EXIT_FAILURE);
	}
	if(signal(SIGSEGV,SignalHandler)==SIG_ERR) {
		perror("problem with calling signal(2)");
		exit(EXIT_FAILURE);
	}
	for(int c=0;c<10;c++) {
		std::cerr << "c is " << c << std::endl;
		try {
			doBadCode(c%2);
		}
		catch(std::exception e) {
			std::cerr << "Got exception, lets continue anyway" << std::endl;
		}
	}
	return EXIT_SUCCESS;
}
