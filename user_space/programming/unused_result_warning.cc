#include <stdio.h>

/*
 *      This is a demo of how to write a function for which the user must check the
 *      return value for...
 *
 *              Mark Veltzer
 *
 * EXTRA_LIBS=
 */

// this is how to use unused in a function declaration...
// this is the right place to put the __attribute__...
int add(int a, int b) __attribute__((warn_unused_result));

// now for the function definition...
int add(int a, int b) {
	return(a + b);
}


int main(int argc, char **argv, char **envp) {
	const int a = 5;
	const int b = 5;

	// the next line will produce a compile time error
	//add(5,6);
	// this is a good line...
	printf("%d+%d is %d\n", a, b, add(a, b));
	return(0);
}
