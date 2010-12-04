#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <unistd.h> // for sysconf

#include "us_helper.hh"

/*
 * This is a demo to show how sync_synchronize() is implemented...
 *
 *              Mark Veltzer
 *
 * EXTRA_LIBS=-lpthread
 */
int main(int argc, char **argv, char **envp) {
	printf("main started\n");
	__sync_synchronize();
	printf("main ended\n");
	return(0);
}
