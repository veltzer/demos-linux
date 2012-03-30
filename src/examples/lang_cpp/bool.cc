#include<stdio.h> // for printf(3)
#include<stdbool.h> // for bool, true and false

#include"us_helper.hh"

/*
 * This is a demo of how to use the _Bool and stdbool.h headers for
 * boolean support in C code.
 *
 * Note that this code MUST be compiled using a C compiler and not a C++
 * compiler since the results will be different. In a C++ compiler,
 * _Bool is bool which is a built in type and true and false are built
 * ins.
 *
 *	Mark Veltzer
 */
int main(int argc, char **argv, char **envp) {
	printf("sizeof(_Bool) is %d\n",sizeof(_Bool));
	printf("sizeof(bool) is %d\n",sizeof(bool));
	printf("_Bool is really %s\n",__stringify(_Bool));
	printf("bool is really %s\n",__stringify(bool));
	printf("true is really %s\n",__stringify(true));
	printf("false is really %s\n",__stringify(false));
	return EXIT_SUCCESS;
}
