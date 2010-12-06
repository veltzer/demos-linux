#include <iostream> // for std:cerr
#include <sys/types.h> // for waitpid(2)
#include <sys/wait.h> // for waitpid(2)
#include <unistd.h> // for close(2), dup(2), execl(3), fork(2)
#include <stdio.h> // for perror(3)

#include "us_helper.hh"

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
	sc(close(1));
	// close the read end of the pipe
	sc(close(fd[0]));
	// setup fd 1 to be correct
	sc(dup2(fd[1],1));
	// execute ls -l
	sc(execl("/bin/ls","/bin/ls","-l",NULL));
}

/*
 * Very similar to the above...
 */

void doChildTwo(int* fd) {
	// close standard input 
	sc(close(0));
	// close the write end of the pipe
	sc(close(fd[1]));
	// setup fd 1 to be correct
	sc(dup2(fd[0],0));
	// execute ls -l
	sc(execl("/usr/bin/wc","/usr/bin/wc","-l",NULL));
}

int main(int argc,char** argv,char** envp) {
	// here is an example of using this construct
	int fd[2];
	sc(pipe(fd));
	int pid1;
	sc(pid1=fork());
	if(pid1==0) {
		doChildOne(fd);
	}
	int pid2;
	sc(pid2=fork());
	if(pid2==0) {
		doChildTwo(fd);
	}
	// close the pipe at the parent
	sc(close(fd[0]));
	sc(close(fd[1]));
	int status;
	sc(waitpid(pid1,&status,0));
	sc(waitpid(pid2,&status,0));
	//std::cerr << "both children died" << std::endl;
	return 0;
}
