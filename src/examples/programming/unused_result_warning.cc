#include <stdlib.h> // for EXIT_SUCCESS

/*
 * This is a demo of how to write a function for which the user must check the
 * return value for...
 *
 * There are many more attributes that you can attach to functions in order to
 * get more checks from the compiler at compile time and get better code. Check
 * 'info gcc' for more details.
 *
 * Note that you can attach the "unused" attribute to variables as below
 * to avoid warnings about them being unused when you really don't want
 * to check the return value.
 *
 * see the discussion in:
 * http://stackoverflow.com/questions/3599160/unused-parameter-warnings-in-c-code
 * http://dbp-consulting.com/tutorials/SuppressingGCCWarnings.html
 *
 *		Mark Veltzer
 */

// this is how to use unused in a function declaration...
// this is the right place to put the __attribute__...
int add(int a, int b) __attribute__((warn_unused_result));

// now for the function definition (you cant put the __attribute__ here)...
int add(int a, int b) {
	return(a + b);
}

int main(int argc, char **argv, char **envp) {
	const int a = 5;
	const int b = 5;

	// the next line will produce a compile time error
	//add(5,6);

	// lets turn off gcc warnings locally to avoid the error...
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wunused-result"
	add(5,6);
	#pragma GCC diagnostic pop
	// another way to ignore the requirement to use the result is to put it in a variable
	// but to mark the variable as "usused"...
	int c __attribute__((unused))=add(a,b);
	return EXIT_SUCCESS;
}
