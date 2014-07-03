#include <stdio.h> // for printf(3)
#include <stdlib.h> // for EXIT_SUCCESS
#include <lowlevel_utils.h> // for getstackpointer()
#include <multiproc_utils.h>	// for my_system()
#include <unistd.h>	// for getpid(2)

/*
	This program tries to find the start address of your stack
*/

int main(int argc, char** argv, char** envp) {
	printf("getstackpointer() returned [%p]\n", getstackpointer());
	pid_t mypid=getpid();
	my_system("cat /proc/%d/maps", mypid);
	//my_system("pmap %d", mypid);
	return EXIT_SUCCESS;
}
