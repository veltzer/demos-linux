#include <stdio.h> // for printf(3)
#include <stddef.h> // for offsetof(3)

/*
 *      watch these macro and see if you can figure out what it does.
 *      we use '1' instead of '0' in these next lines because gcc is smart
 *      enough to understand that using 0 will mean that we use the NULL
 *      object and not let us do anything with it including taking fields
 *      or doing pointer arithmetic...
 *      Notice that with my macro, offsetof and __builtin_offsetof you can use the
 *      "struct" prefix to the structure name or drop it.
 *
 *                                              Mark Veltzer
 * EXTRA_LIBS=
 */

#define myoffsetof(structname,fieldname) ((char *)(&(((structname *)1)->fieldname)) - (char *)1)

// our own struct for the demo...
typedef struct _mystruct {
	int field_a;
	int field_b;
	int field_c;
} mystruct;

int main(int argc,char** argv,char** envp) {
	// using our own macro
	printf("field_a is in offset %d\n",myoffsetof(mystruct,field_a));
	printf("field_b is in offset %d\n",myoffsetof(mystruct,field_b));
	printf("field_c is in offset %d\n",myoffsetof(mystruct,field_c));
	// using a libc macro
	printf("field_a is in offset %d\n",offsetof(mystruct,field_a));
	printf("field_b is in offset %d\n",offsetof(mystruct,field_b));
	printf("field_c is in offset %d\n",offsetof(mystruct,field_c));
	// using a gcc builtin
	printf("field_a is in offset %d\n",__builtin_offsetof(mystruct,field_a));
	printf("field_b is in offset %d\n",__builtin_offsetof(mystruct,field_b));
	printf("field_c is in offset %d\n",__builtin_offsetof(mystruct,field_c));
	// using a gcc builtin
	printf("field_a is in offset %d\n",__compiler_offsetof(mystruct,field_a));
	printf("field_b is in offset %d\n",__compiler_offsetof(mystruct,field_b));
	printf("field_c is in offset %d\n",__compiler_offsetof(mystruct,field_c));
	return(0);
}
