#include <stdio.h> // for printf(3)
#include <stdlib.h> // for EXIT_SUCCESS

/*
 * This is an example of getting a register on an i32 machine
 *
 *		Mark Veltzer
 */

static inline unsigned int getrdtsc() {
	unsigned int val;
	asm ("rdtsc" : "=val" (val));
	return val;
}
static inline unsigned int getstackpointer() {
	unsigned int val;
	asm ("movl %%esp, %0" : "=r" (val));
	return val;
}

int main(int argc, char **argv, char **envp) {
	printf("stackpointer is %u\n",getstackpointer());
	printf("rdtsc is %u\n",getrdtsc());
	return EXIT_SUCCESS;
}
