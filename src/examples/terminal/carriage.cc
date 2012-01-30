#include <stdio.h> // for printf(3), fflush(3), setvbuf(3)
#include <unistd.h> // for usleep(3)

#include "us_helper.hh"

/*
 *      This is an example of how to use the carriage return character
 *      to do some nice output progress reporting without scrolling with standard
 *      output...
 *
 *      Notes:
 *      - we print space after printing to show the general way to use this since our
 *      number is going down and not up. In the up case you would not need it.
 *      - the 'fflush' is actually needed. Otherwise you would still get updates but
 *      only when stdout is flushed naturally which happens only when it's buffer
 *      is full or when "\n" is printed.
 *	- you can set stdout for no buffering, use stderr instead, or use direct write(2)
 *	to avoid the extra call to flush.
 *	- ofcouse, you can also use the curses library to do all of this which has a much
 *	richer API but does require some learning.
 *
 *              Mark Veltzer
 */
int main(int argc, char **argv, char **envp) {
	printf("stdout, no flush (bad updates)\n");
	for (unsigned int i = 1000; i > 0; i--) {
		printf("i is %d      \r", i);
		usleep(10000);
	}
	printf("\n\n");
	printf("stdout, with flush (good updates, overhead on calling flush)\n");
	for (unsigned int i = 1000; i > 0; i--) {
		printf("i is %d      \r", i);
		fflush(stdout);
		usleep(10000);
	}
	printf("\n\n");
	printf("stdout, no buffer (good updates, no call to flush so good performance)\n");
	sc(setvbuf(stdout,NULL,_IONBF,0));
	for (unsigned int i = 1000; i > 0; i--) {
		printf("i is %d      \r", i);
		usleep(10000);
	}
	printf("\n\n");
	printf("stderr, unbuffered naturally so no flush\n");
	for (unsigned int i = 1000; i > 0; i--) {
		fprintf(stderr,"i is %d      \r", i);
		usleep(10000);
	}
	printf("\n\n");
	return(0);
}
