#include <stdio.h>  // for printf(3)
#include <unistd.h> // for sleep(3)

/*
 *      Explore usage of command line gcore.
 *
 *      This simple process simply increases a counter and prints it in a slow
 *      manner.
 *
 *      Take a photo of this process in action using gcore and then debug it
 *      and see if you get the right value...
 *
 *              Mark Veltzer
 *
 * EXTRA_LIBS=
 */
int main(int argc, char **argv, char **envp) {
	for (unsigned int i = 0; i < 10000; i++) {
		printf("i is %d\n", i);
		sleep(1);
	}
	return(0);
}
