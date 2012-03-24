#include <stdio.h> // for printf(3)
#include <stdlib.h> // for atoi(3)

/*
 * Lets see if short circuit bool expression evaluation works...
 * You determine (via args) whether the first expression or second
 * expression is 1 or 0.
 * The reason that I'm using argv here is in order for the compilers
 * optimizer not to know ahead of time what the result would be...
 *
 *		Mark Veltzer
 */
int do_something(int val) {
	printf("Im doing something...\n");
	return(val);
}

int main(int argc, char **argv, char **envp) {
	if (argc != 3) {
		printf("usage: %s [val1] [val2]\n", argv[0]);
		return(-1);
	}
	if (atoi(argv[1]) || do_something(atoi(argv[2]))) {
		printf("Inside the if statement\n");
	} else {
		printf("Outside the if statement\n");
	}
	return(0);
}
