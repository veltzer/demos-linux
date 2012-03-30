#include<curses.h>
#include<stdlib.h> // for EXIT_SUCCESS

/*
 * Curses example program number 2...
 *
 * This is an example of how to read unbufferd input from stdin...
 * In this example we want to read JUST A SINGLE character...
 *
 * This example uses curses to achieve this goal although other
 * tools may be used to achieve the same goal
 * (system calls for instance).
 *
 *		Mark Veltzer
 *
 * EXTRA_LIBS=-lncursesw
 */
int main(int argc, char **argv, char **envp) {
	initscr();
	cbreak();
	noecho();
	return EXIT_SUCCESS;
}
