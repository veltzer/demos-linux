#include "us_helper.hh"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
 *      This demo shows several different things:
 *      1. If you define a 'const' pointer in C++ then you can assign any
 *      other const pointer of the same type to it.
 *      2. The compiler issues an error whenever your program tries to write to
 *      the const pointer.
 *      3. You can force acess to a "const char*" string storage via pointer manipulation.
 *      The compiler will not stop you there.
 *      4. If you do try to access that memory space you will get a segfault.
 *      5. You get the segfault because constant strings are allocated in read only memory.
 *      6. The fact that constat strings are allocated in read only memory allows the
 *      compiler to optimize this memory and never store the same string twice.
 *      7. If you optimize your program (-O2) then the compiler makes some of your memory
 *      access functions go away and so you don't segfault.
 *
 *              Mark Veltzer
 */
int main(int argc, char **argv, char **envp) {
	const char *other = "blabla";
	const char *other2 = "blabla";
	const char *foo = "bar";

	printproc(NULL);

	printf("foo is %p\n", foo);
	printf("other is %p\n", other);
	printf("other2 is %p\n", other2);

	// this does NOT generate a compile error!
	foo = other;

	printf("foo is %p\n", foo);
	printf("other is %p\n", other);
	// next line will cause the following error:
	// --- char_pointer.cc:15: error: assignment of read-only location ‘*(foo + 2u)’
	//foo[2]='y';

	printf("foo is %s\n", foo);
	printf("foo[2] is %c\n", foo[2]);

	// now lets manipulathe the data. The casting is neccessary to avoid compile error.
	char *p = (char *)foo;
	printf("p is %p\n", p);
	//sprintf(p,"hello");
	p[2] = 'y';
	printf("p is %s\n", p);
	printf("foo is %s\n", foo);
	printf("foo[2] is %c\n", foo[2]);
	return(0);
}
