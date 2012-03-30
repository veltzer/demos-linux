#include<stdio.h>
#include<string.h>

/*
 *	This is to test octal use in user space...
 *
 *		Mark Veltzer
 */
int main(int argc, char **argv, char **envp) {
	int i = 0777;

	printf("i is %d\n", i);
	return(0);
}
