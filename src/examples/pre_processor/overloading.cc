#include <stdio.h> // for printf(3)
#include <stdlib.h> // for EXIT_SUCCESS

/*
 * This example shows that you cannot redefine preprocessor macros.
 *
 *		Mark Veltzer
 */
#define mymacro(a1) printf("one\n");
// the next line will cause a compile time error...
//#define mymacro(a1,a2) printf("two\n");
int main(int argc, char **argv, char **envp) {
	return EXIT_SUCCESS;
}
