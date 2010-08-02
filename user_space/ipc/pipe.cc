#include<iostream> // for std:cerr
#include<sys/types.h> // for waitpid(2)
#include<sys/wait.h> // for waitpid(2)
#include<unistd.h> // for close(2), dup(2), execl(3), fork(2)
#include<stdio.h> // for perror(3)

// Here is a template that will take care of
// calling system calls and throwing an exception if something bad happens.

template<class T> T syscall(T val,T err_val) {
	if(val==err_val) {
		perror("C++ exception thrown");
		throw std::exception();
	}
	return val;
}

/*
 * This is the first child
 * It closes it's own standrad output (which it inherited from the parent - usually
 * 	the console where the parent was run).
 * It then closes the side of the pipe that it is not going to use (the read side [0]).
 * 	This is done so that the other side will get SIGPIPE and eof when all is done.
 * It then duplicates the side of the pipe that it will use (the write side [1]) to its
 * 	standard output (that it will give to it's children in case of exec).
 * It executes ls which prints to the pipes write side.
 * 	When ls dies this process dies.
 * 	The line after execl doesn't get executed...
 */

void doChildOne(int* fd) {
	// close standard output 
	syscall(close(1),-1);
	// close the read end of the pipe
	syscall(close(fd[0]),-1);
	// setup fd 1 to be correct
	syscall(dup2(fd[1],1),-1);
	// execute ls -l
	syscall(execl("/bin/ls","/bin/ls","-l",NULL),-1);
}

/*
 * Very similar to the above...
 */

void doChildTwo(int* fd) {
	// close standard input 
	syscall(close(0),-1);
	// close the write end of the pipe
	syscall(close(fd[1]),-1);
	// setup fd 1 to be correct
	syscall(dup2(fd[0],0),-1);
	// execute ls -l
	syscall(execl("/usr/bin/wc","/usr/bin/wc","-l",NULL),-1);
}

int main(int argc,char** argv,char** envp) {
	// here is an example of using this construct
	int fd[2];
	syscall(pipe(fd),-1);
	int pid1=syscall(fork(),-1);
	if(pid1==0) {
		doChildOne(fd);
	}
	int pid2=syscall(fork(),-1);
	if(pid2==0) {
		doChildTwo(fd);
	}
	// close the pipe at the parent
	syscall(close(fd[0]),-1);
	syscall(close(fd[1]),-1);
	int status;
	syscall(waitpid(pid1,&status,0),-1);
	syscall(waitpid(pid2,&status,0),-1);
	std::cerr << "both children died" << std::endl;
	return 0;
}
