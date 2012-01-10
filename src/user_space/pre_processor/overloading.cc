#include <stdio.h> // for printf(3)

/*
 * This example shows that you cannot redefine preprocessor macros.
 *
 *              Mark Veltzer
 *
 * EXTRA_LIBS=
 */
#define mymacro(a1) printf("one\n");
// the next line will cause a compile time error...
//#define mymacro(a1,a2) printf("two\n");
int main(int argc, char **argv, char **envp) {
	return(0);
}
