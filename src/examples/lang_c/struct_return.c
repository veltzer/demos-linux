#include<stdio.h> // for printf(3)
#include<stdlib.h> // for EXIT_SUCCESS

/*
 * This is an attempt to return a struct from a function
 *
 * Mark Veltzer
 */

typedef struct _foo {
	int a;
	int b;
} foo;

foo giveMeFoo(void) {
	// following line causes a compilation error...
	//return {3,4};
	foo f;
	return(f);
}

int main(int argc, char** argv, char** envp) {
	foo f=giveMeFoo();
	printf("f.a is %d\n",f.a);
	printf("f.b is %d\n",f.b);
	return EXIT_SUCCESS;
}
