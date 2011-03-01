#include <stdio.h> // for fprintf(3)

/*
 *	This is an example of a memory barrier
 *	This example needs to be compiled with optimizations to see it in action.
 *	View the resulting assembly code with:
 *	objdump --disassemble --line-numbers --source --demangle barrier
 *
 *	TODO:
 *
 *		Mark Veltzer
 *
 * EXTRA_LIBS=
 */

int main(int argc, char **argv, char **envp) {
	int a = 0;
	int u = 0;
	int vals[10];
	int loc=0;
	// p will point to a but the compiler does not know it.
	int *p = &u + 1;
	// this taking of the address of a to have the compiler actually store
	// a on the stack at all! If we don't do this the compiler will treat
	// a as a register for the entire scope of this function!
	int *pa = &a;

	// this printing is essential to keep the compiler from telling us
	// that 'pa' is an unused variable...
	fprintf(stderr, "pa is %p\n", pa);
	//fprintf(stderr,"p is %p\n",p);
	//fprintf(stderr,"&a is %p\n",&a);
	//this code will cause the compiler to put a into a register
	for (int i = 0; i < 100; i++) {
		a++;
	}
	// the compiler writes a to it's 'natural' place on the stack
	*p = 999;
	// suprisingly, the compiler is certain that the register is still
	// holding the right value for a and so uses it in the print...
	vals[loc++]=a;
	// this line does NOT write the value of the register used to hold a into
	// the ram place for a on the stack and the proof for this is the fact
	// that 999 is printed for a. It looks like the compiler is certain that
	// the memory location for a IS synchronized with the register value for a
	// bacause it wrote it to memory after the 'for' loop.
	vals[loc++]=a;
	vals[loc++]=*p;

	while (a < 3000) {
		a++;
	}
	*p = 2000;
	// if you remove the following __sync_synchronize then the results
	// will change...
	vals[loc++]=a;
	// this is a compiler barrier which forces not looking at registers
	// after it...
	__sync_synchronize();
	vals[loc++]=a;
	
	while (a < 3000) {
		a++;
	}
	*p = 2000;
	// if you remove the following __sync_synchronize then the results
	// will change...
	vals[loc++]=a;
	// a function call is actually a barrier...
	fprintf(stderr,"hello\n");
	vals[loc++]=a;

	// print the results
	for(int i=0;i<loc;i++) {
		fprintf(stderr, "val is %d\n",vals[i]);
	}
	return(0);
}
