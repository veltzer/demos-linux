#include<stdio.h> // for printf(3)
#include<stdlib.h> // for EXIT_SUCCESS

/*
 * This example shows how to quote strings in GCC
 * Stolen from the linux kernel.
 * This MUST be done in 2 levels...
 *
 *		Mark Veltzer
 */
#define __stringify_1(x) # x
#define __stringify(x) __stringify_1(x)
#define FOOBAR 2.2 .2

int main(int argc, char **argv, char **envp) {
	printf("this is some " __stringify(FOOBAR) " string\n");
	return EXIT_SUCCESS;
}
