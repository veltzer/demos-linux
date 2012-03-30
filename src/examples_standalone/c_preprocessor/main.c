/*
 * This is an example shows how to determine an include file at compile time
 * in a way which is dependant on compile time macros. You could do this using
 * #ifdef but the idea is that you do not want to change this could as you add
 * more include files.
 *
 * Stolen from compiler-gcc.h from the kernel.
 *
 * 	Mark
 */

#include<stdio.h> // for printf(3)

#define __quote(x) #x
#define _myheader(y) __quote(header_##y.h)
#define myheader(x) _myheader(x)
#includemyheader(TYPE)

int main(int argc,char** argv,char** envp) {
	printf("SYMBOL is %s\n",SYMBOL);
	return 0;
}
