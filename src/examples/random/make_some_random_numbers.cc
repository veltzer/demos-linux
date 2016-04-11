#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv, char** envp) {
	srandom(atoi(argv[1]));
	for(int i=0; i<10;i++) {
		printf("%ld\n", random());
	}
	return EXIT_SUCCESS;
}
