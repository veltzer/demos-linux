#include"us_helper.hh"

/*
 * This is a simple example causing a segmentation fault to be generated.
 *
 * The address "0" is not mapped into any regular processes virtual address space
 * which is a good thing since NULL (which is heavily used in C programs) has the
 * value 0.
 *
 * 				Mark Veltzer
 *
 * TODO:
 * - show that you can catch SIGSEGV and even continue running.
 */

int main(int argc, char **argv, char **envp) {
	// lets show that the NULL constant is actually the number 0...
	TRACE("NULL is %d",NULL);
	char *p = 0;
	p[0] = 0;
	return EXIT_SUCCESS;
}
