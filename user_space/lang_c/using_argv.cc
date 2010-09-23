#include <stdio.h>
#include <string.h>

int main(int argc, char **argv, char **envp) {
	if (strcmp(argv[0], "./add") == 0) {
		printf("Im adding");
	}
	if (strcmp(argv[0], "./sub") == 0) {
		printf("Im subtracting");
	}
	return(0);
}
