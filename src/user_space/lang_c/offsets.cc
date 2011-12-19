#include <stdio.h> // for printf(3)

/*
 *      watch these macro and see if you can figure out what it does.
 *      we use '1' instead of '0' in these next lines because gcc is smart
 *      enough to understand that using 0 will mean that we use the NULL
 *      object and not let us do anything with it including taking fields
 *      or doing pointer arithmetic...
 *
 *                                              Mark Veltzer
 * EXTRA_LIBS=
 */

#define myoffsetof(fieldname,structname) ((char *)(&(((structname *)1)->fieldname)) - (char *)1)

// our own struct for the demo...
struct mystruct {
	int field_a;
	int field_b;
	int field_c;
};

int main(int argc,char** argv,char** envp) {
	// using our own macro
	printf("field_a is in offset %d\n",myoffsetof(field_a,mystruct));
	printf("field_b is in offset %d\n",myoffsetof(field_b,mystruct));
	printf("field_c is in offset %d\n",myoffsetof(field_c,mystruct));
	// using a libc macro
	printf("field_a is in offset %d\n",offsetof(field_a,mystruct));
	printf("field_b is in offset %d\n",offsetof(field_b,mystruct));
	printf("field_c is in offset %d\n",offsetof(field_c,mystruct));
	// using a gcc builtin
	printf("field_a is in offset %d\n",__builtin_offsetof(field_a,mystruct));
	printf("field_b is in offset %d\n",__builtin_offsetof(field_b,mystruct));
	printf("field_c is in offset %d\n",__builtin_offsetof(field_c,mystruct));
	return(0);
}
