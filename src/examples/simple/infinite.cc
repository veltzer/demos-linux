#include <stdlib.h> // for EXIT_SUCCESS

/*
 * This is a demo of a program always wanting the CPU.
 * If you look at it through ps like in:
 * ps -o comm,pid,state -e | grep infinite
 * you will see it in a running state.
 *
 * 				Mark Veltzer
 */

int main(int argc,char** argv,char** envp) {
	while(true) {
	}
	return EXIT_SUCCESS;
}
