#include <stdio.h>
#include "libadd/add.h"

int main(int argc, char **argv, char **envp) {
	printf("Hello, World!\n");
	int c = add(2, 2);
	printf("c is %d\n", c);
	return(0);
}
