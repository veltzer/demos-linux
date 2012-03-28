#include<stdio.h>

/*
 * This is an example showing how to force the compiler not to inline the function.
 * How can you check that this works? compare the two disassembled version
 * resulting from putting the "noinline" line into a comment
 *
 *		Mark Veltzer
 */

int doit(int a,int b) __attribute__((noinline));
int doit(int a,int b) {
	return a+b;
}

int main(int argc,char** argv,char** envp) {
	printf("did you know that 2+2=%d\n",doit(2,2));
	return 0;
}
