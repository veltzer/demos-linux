#include <stdio.h> // for printf(3)
#include <stddef.h> // for offsetof(3)

/*
 *	This is an example of finding out how far a field is from the begining
 *	of the structure it is embedded in.
 *      In a similar example in C++ (using g++ instead of gcc) we had to
 *      use '1' instead of '0' in these next macros because g++ is smarter
 *      than gcc and translated the 0 to NULL and disallowed it use.
 *      In this example, since we are compiling to C we can use 0 freely.
 *      Notice that with my macro, offsetof and __builtin_offsetof you can use the
 *      "struct" prefix to the structure name or drop it.
 *
 *                                              Mark Veltzer
 */

#define myoffsetof(structname,fieldname) ((char *)(&(((structname *)0)->fieldname)) - (char *)0)

// our own struct for the demo...
typedef struct _mystruct {
	int field_a;
	int field_b;
	int field_c;
} mystruct;

int main(int argc,char** argv,char** envp) {
	// using our own macro
	printf("field_a is in offset %d\n",myoffsetof(mystruct,field_a));
	printf("field_b is in offset %d\n",myoffsetof(struct _mystruct,field_b));
	printf("field_c is in offset %d\n",myoffsetof(mystruct,field_c));
	// using a libc macro
	printf("field_a is in offset %d\n",offsetof(mystruct,field_a));
	printf("field_b is in offset %d\n",offsetof(struct _mystruct,field_b));
	printf("field_c is in offset %d\n",offsetof(mystruct,field_c));
	// using a gcc builtin
	printf("field_a is in offset %d\n",__builtin_offsetof(mystruct,field_a));
	printf("field_b is in offset %d\n",__builtin_offsetof(struct _mystruct,field_b));
	printf("field_c is in offset %d\n",__builtin_offsetof(mystruct,field_c));
	return(0);
}
