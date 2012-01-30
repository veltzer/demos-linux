#include <stdio.h> // for printf(3)

/*
 *      This is a demo of how to write a function for which the user should not use
 *      (deprecated). 
 *
 *      There are many more attributes that you can attach to functions in order to
 *      get more checks from the compiler at compile time and get better code. Check
 *      'info gcc' for more details.
 *
 *              Mark Veltzer
 */

// this is how to use unused in a function declaration...
// this is the right place to put the __attribute__...
//int add(int a, int b) __attribute__((deprecated));

// now for the function definition (you cant put the __attribute__ here)...
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
