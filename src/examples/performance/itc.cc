#include <stdio.h> // for printf(3)
#include <unistd.h> // for sleep(3)

//#include "us_helper.hh"

/*
 * Demo for a performance counter on i64...
 *
 *		Mark Veltzer
 */

typedef unsigned long ticks;

static __inline__ ticks getticks(void) {
	ticks ret;
	// this does not compile for some reason (stolen from the PAPI library...)
	//asm __volatile__("mov %0=ar.itc":"=r" (ret));
	ret=1;
	return ret;
}

int main(int argc, char **argv, char **envp) {
	printf("starting up...\n");
	printf("ticks is %lu\n",getticks());
	sleep(1);
	printf("ticks is %lu\n",getticks());
	return(0);
}
