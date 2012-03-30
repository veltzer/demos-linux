#include <iostream> // for std::cout, std::endl
#include <stdlib.h> // for EXIT_SUCCESS

/*
 * This is a simple example of how to create a simple floating point exception.
 * The exception is a ** machine hardware level ** exception which cause the CPU
 * to fault, go to the OS, the OS sends a SIGFPE to your program, you can respond
 *
 * 				Mark Veltzer
 */

int main(int argc, char **argv, char **envp) {
	int i = 0;
	int j = 1 / i;

	std::cout << j << std::endl;
	return EXIT_SUCCESS;
}
