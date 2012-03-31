#include<stdio.h> // for printf(3)
#include<stdlib.h> // for atoi(3)

/*
 * A simple example of using the atoi library function
 *
 * 			Mark Veltzer
 */

int main(int argc, char **argv, char **envp) {
	const char* str="321";
	printf("the value of the string is %d\n",atoi(str));
	return 0;
}
