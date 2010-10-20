#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "us_helper.hh"

/*
 *      This example shows how you can write C functions in C++ code.
 *      This seems trivial, right ?!? Just write regular functions
 *      like:
 *      =========== SNIP =========
 *      int add(int a,int b) {
 *              return a+b;
 *      }
 *      =========== SNIP =========
 *      But this is not what I mean....
 *      I want the linkage to be C style...
 *
 *      The assumption in this example is that it is compiled using a C++
 *      compiler...
 *
 *              Mark Veltzer
 *
 * EXTRA_LIBS=
 */

/*
 * The class here is just to make sure that this code does not get compiled by a C
 * compiler but rather by a C++ compiler.
 *
 * TODO:
 * Move to checking this using #ifdef cplusplus and #error.
 */
class A {
public:
	int a;
};

// this is a function of C++ code linked in C++ style
int cpp_style_add(int a, int b) {
	return(a + b);
}


// this is a function of C++ code linked in C style
// notice that you can still write C code in this function...
extern "C" int c_style_add(int a, int b) {
	// the next line will produce an error
	A *obj = new A();

	obj->a = 0;
	return(a + b + obj->a);
}


int main(int argc, char **argv, char **envp) {
	printf("These are the symbols without demangling:\n");
	my_system("nm ./c_in_cpp | grep style_add");
	printf("These are the symbols with demangling:\n");
	my_system("nm -C ./c_in_cpp | grep style_add");
	return(0);
}
