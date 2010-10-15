#include <stdio.h> // for fprintf(3)

/*
 *      This is an example of a memory barrier
 *      This example needs to be compiled with optimizations to see it in action.
 *      View the resulting assembly code with:
 *      objdump --disassemble --line-numbers --source --demangle barrier
 *
 *      TODO:
 *      - store a instead of printing it and show the various values.
 *      - try out with different inlining attributes of the print_int function.
 *
 *              Mark Veltzer
 *
 * EXTRA_LIBS=
 */
void print_int(int a) {
	fprintf(stderr, "int is %d\n", a);
}


int main(int argc, char **argv, char **envp) {
	int a = 0;
	int u = 0;
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
	// suprisnlgy, the compiler is certain that the register is still
	// holding the right value for a and so uses it in the print...
	print_int(a);
	// this line does NOT write the value of the register used to hold a into
	// the ram place for a on the stack and the proof for this is the fact
	// that 999 is printed for a. It looks like the compiler is certain that
	// the memory location for a IS synchronized with the register value for a
	// bacause it wrote it to memory after the 'for' loop.
	__sync_synchronize();
	print_int(a);
	while (a < 3000) {
		a++;
	}
	*p = 2000;
	print_int(a);
	__sync_synchronize();
	print_int(a);
	return(0);
}
