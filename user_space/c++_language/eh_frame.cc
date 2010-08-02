#include <stdio.h>
#include <string.h>

/*
 *      This example explores the funky eh_frame
 *
 *              Mark Veltzer
 *
 * EXTRA_LIBS=
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
	return(0);
}
