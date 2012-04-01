#include<stdio.h> // for fprintf(3)
#include<stdlib.h> // for EXIT_SUCCESS

/*
 * This is an empty testing application
 */
int main(int argc, char **argv, char **envp) {
	fprintf(stderr, "Hello, World!\n");
	return EXIT_SUCCESS;
}
