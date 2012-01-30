#include<iostream>
#include<signal.h>

// Here is a template that will take care of all out needs

/*
int syscall(int val) {
	if(val==-1) {
		perror("C++ exception thrown");
		throw std::exception();
	}
	return val;
}
*/

template<class T> T syscall(T val,T err_val) {
	if(val==err_val) {
		perror("C++ exception thrown");
		throw std::exception();
	}
	return val;
}

void myhandler(int sig) {
	std::cerr << "in signal handler for signal " << sig << std::endl;
}

int main(int argc,char** argv,char** envp) {
	// here is an example of using this construct
	int fd[2];
	try {
		syscall(pipe(fd),-1);
		syscall(signal(SIGPIPE,myhandler),SIG_ERR);
	}
	catch(std::exception e) {
		std::cerr << "cought exception" << std::endl;
	}
	return 0;
}
