#include <stdio.h> // for printf(3)
#include <sys/utsname.h> // for uname(2)
#include <stdbool.h> // for bool

#include "us_helper.hh" // our own helper

/*
 *      This is a demo showing the sizes of variables on the architecture
 *      it is running on (it also prints the name of the architecture).
 *
 *      This is intended for a GNU/linux system.
 *
 *              Mark Veltzer
 *
 * EXTRA_LIBS=
 */
#define PRINT_SIZEOF(type) printf("size of " __stringify(type) " is %d\n", sizeof(type));

int main(int argc, char **argv, char **envp) {
	struct utsname buf;

	sc(uname(&buf));
	printf("architecture is [%s]\n", buf.machine);
	PRINT_SIZEOF(bool);
	PRINT_SIZEOF(char);
	PRINT_SIZEOF(unsigned char);
	PRINT_SIZEOF(short);
	PRINT_SIZEOF(unsigned short);
	PRINT_SIZEOF(int);
	PRINT_SIZEOF(unsigned int);
	PRINT_SIZEOF(long);
	PRINT_SIZEOF(unsigned long);
	PRINT_SIZEOF(long long);
	PRINT_SIZEOF(unsigned long long);
	PRINT_SIZEOF(float);
	PRINT_SIZEOF(double);
	// this is not allowed in C
	//float* x={ 5, 3, 2, 1 }; 
	// but this is...
	float x[] = { 5, 3, 2, 1 }; 
	printf("size of x is %d\n",sizeof(x));
	return(0);
}
