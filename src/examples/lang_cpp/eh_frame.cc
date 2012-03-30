#include<stdio.h> // for printf(3)
#include<stdlib.h> // for EXIT_SUCCESS

/*
 * This example explores the funky eh_frame
 *
 * TODO:
 *	What does this demo do ? try to find out and document it...
 *
 *		Mark Veltzer
 */

class A {
public:
	static void inner(void) {
		printf("Hello\n");
	}


	void doit(void) {
		inner();
	}
};

int main(int argc, char **argv, char **envp) {
	A a;
	a.doit();
	return EXIT_SUCCESS;
}
