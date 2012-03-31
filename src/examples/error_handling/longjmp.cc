#include<stdio.h> // for fprintf(3)
#include<setjmp.h> // for longjmp(3), setjmp(3)
#include<stdlib.h> // for EXIT_SUCCESS

/*
 * This is a demo of how to use the setjmp/longjmp in order to simulate exception
 * handling in C code.
 *
 * Mark Veltzer
 */

// This buffer saves the context of the jump. We need one for each unique
// point we wish to jump to
static jmp_buf env;

/* this simulates a function which sometimes encounters errors */
void func() {
	static int counter=0;

	counter++;
	if (counter % 3 == 0) {
		longjmp(env, counter);
	}
	fprintf(stderr, "this is the continuation of the function\n");
}


int main(int argc, char **argv, char **envp) {
	for(int c=0;c<10;c++) {
		int ret;
		ret=setjmp(env);
		if(!ret) {
			// This is the regular code. We get here when setting doing the
			// setjmp for the first time
			fprintf(stderr, "c is %d\n", c);
			func();
		} else {
			// we got here from longjmp, this means that some
			// error occured
			fprintf(stderr, "oops, got an error from func %d\n", ret);
		}
	}
	return EXIT_SUCCESS;
}
