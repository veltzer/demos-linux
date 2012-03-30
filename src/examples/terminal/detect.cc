#include<stdio.h> // for stdin
#include<unistd.h> // for isatty, ttyname

#include"us_helper.hh"

/*
 * This is an example of how to detect whether a file you are using is
 * a terminal or not and print it's name in case it is.
 *
 *		Mark Veltzer
 */
int main(int argc, char **argv, char **envp) {
	// lets take the file descriptor number from stdin which is usually
	// a terminal (unless you redirect it...)
	int filedes = fileno(stdin);

	if (isatty(filedes)) {
		DEBUG("it is a terminal with name [%s]", ttyname(filedes));
	} else {
		DEBUG("it is not a terminal");
	}
	return EXIT_SUCCESS;
}
