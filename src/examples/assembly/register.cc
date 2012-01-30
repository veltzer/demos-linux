#include <stdio.h> // for printf(3)

/*
 * This is an example of getting a register on an i32 machine
 *
 *              Mark Veltzer
 */

static inline unsigned int getregister() {
	unsigned int val;

	asm ("rdtsc" : "=val" (val));
	return val;
}

int main(int argc, char **argv, char **envp) {
	unsigned int val=getregister();
	printf("val is %u\n",val);
	return(0);
}
