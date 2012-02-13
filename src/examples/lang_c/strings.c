#include <stdio.h> // for printf(3)

/*
 * This is a demo showing what happens if strings span more than one line
 *
 *              Mark Veltzer
 */

int main(int argc, char **argv, char **envp) {
	// the next line will create a compile time warning or error in most modern
	// compilers (earlier compilers allowed this).
	//printf("this is a string
	//	spanning more than one line\n");
	// C catenates two strings into one, no space or newline is put in the middle...
	printf("this is a string"
		" spanning more than one line\n");
	return(0);
}
