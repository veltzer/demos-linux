#include <stdio.h> // for printf(3)
#include <unistd.h> // for sleep(3)

//#include "us_helper.hh"

/*
 * Demo for a performance counter on i64...
 *
 *		Mark Veltzer
 */

static inline unsigned long getstackpointer(void) {
	unsigned long ret;
	asm ("movl %%esp, %0" : "=r" (ret));
	return ret;
}

int main(int argc, char** argv, char** envp) {
	printf("stackpointer is %lu\n",getstackpointer());
	sleep(1);
	printf("stackpointer is %lu\n",getstackpointer());
	return(0);
}
