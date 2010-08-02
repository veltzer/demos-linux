#include <stdio.h>
#include <string.h>
#include <unistd.h>

/*
 *      This is an example of how to use the carriage return character
 *      to do some nice output without scrolling with standard stdio graphics...
 *
 *              Mark Veltzer
 *
 * EXTRA_LIBS=
 */
int main(int argc, char **argv, char **envp) {
	for (unsigned int i = 0; i < 10000; i++) {
		printf("i is %d        \r", i);
		fflush(stdout);
		usleep(10000);
	}
	return(0);
}
