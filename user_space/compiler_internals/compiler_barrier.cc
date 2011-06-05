#include <stdio.h> // for printf(3)
#include <stdlib.h> // for srandom(3)

/*
 *	This is an example of a compiler barrier
 *	This example needs to be compiled with optimizations to see it in action.
 *	View the resulting assembly code with:
 *	objdump --disassemble --line-numbers --source --demangle barrier
 *
 *	The gcc compiler barrier
 *	is the asm(memory) one. It is not a CPU reordering barrier - just a compiler
 *	one.
 *	It seems that gcc does not have the notion of a *read* vs *write*
 *	barrier which could come in handly instead of this dominating
 *	*FULL* barrier.
 *
 *	Machine memory barriers
 *	is the _sync_synchronize() one. There doesn't seem to be a read vs write
 *	one.
 *
 *	TODO:
 *	- make this program show the instructions that are emitted for main so
 *	that people could see the assembly code generated.
 *	- do another example that shows, for example, that two consecutive reads
 *	from the same address are optimized away by the compiler (the second read
 *	that is).
 *	- add another example that shows why volatile is bad (base it on the kernel
 *	article about volatiles being harmful).
 *
 *		Mark Veltzer
 *
 * EXTRA_LIBS=
 */

int main(int argc, char **argv, char **envp) {
	int a = 0;
	int u = 0;
	int val_before[10];
	int val_after[10];
	int loc=0;
	// p will point to a but the compiler does not know it.
	int *p = &u + 1;
	// this taking of the address of a to have the compiler actually store
	// a on the stack at all! If we don't do this the compiler will treat
	// a as a register for the entire scope of this function!
	int *pa = &a;

	// this printing is essential to keep the compiler from telling us
	// that 'pa' is an unused variable...
	printf("pa is %p\n", pa);
	printf("p is %p\n",p);
	printf("&a is %p\n",&a);
	printf("now starting...\n");

	// FIRST EXAMPLE (NO BARRIER)	
	// ==========================
	//this code will cause the compiler to put a into a register
	a=1000;
	while (a < 3000) {
		a+=a;
	}
	// the compiler writes a to it's 'natural' place on the stack
	*p = 2000;
	// suprisingly, the compiler is certain that the register is still
	// holding the right value for a and so uses it in the print...
	val_before[loc]=a;
	// this line does NOT write the value of the register used to hold a into
	// the ram place for a on the stack and the proof for this is the fact
	// that 999 is printed for a. It looks like the compiler is certain that
	// the memory location for a IS synchronized with the register value for a
	// bacause it wrote it to memory after the 'for' loop.
	val_after[loc]=a;
	loc++;
	//vals[loc++]=*p;

	// SECOND EXAMPLE (__sync_synchronize() BARRIER)
	// =============================================
	a=1000;
	while (a < 3000) {
		a+=a;
	}
	*p = 2000;
	// if you remove the following __sync_synchronize then the results
	// will change...
	val_before[loc]=a;
	// this is a compiler barrier which forces not looking at registers
	// after it...
	__sync_synchronize();
	val_after[loc]=a;
	loc++;

	// THIRD EXAMPLE (function call barrier)
	// =====================================
	a=1000;
	while (a < 3000) {
		a+=a;
	}
	*p = 2000;
	// if you remove the following __sync_synchronize then the results
	// will change...
	val_before[loc]=a;
	// a function call is actually a barrier...
	srandom(100);
	val_after[loc]=a;
	loc++;
	
	// FOURTH EXAMPLE (assembly memory barrier)
	// ================================
	a=1000;
	while (a < 3000) {
		a+=a;
	}
	*p = 2000;
	// if you remove the following __sync_synchronize then the results
	// will change...
	val_before[loc]=a;
	// a statement telling the GNU assembler not to reorder around it
	asm volatile("" ::: "memory");
	val_after[loc]=a;
	loc++;

	// FIFTH EXAMPLE (empty assembly block)
	// ================================
	a=1000;
	while (a < 3000) {
		a+=a;
	}
	*p = 2000;
	// if you remove the following __sync_synchronize then the results
	// will change...
	val_before[loc]=a;
	// a statement telling the GNU assembler not to reorder around it
	asm volatile("");
	val_after[loc]=a;
	loc++;

	const char* test_names[]={
		"no barrier",
		"sync_synchronize",
		"function call",
		"assembly memory barrier",
		"assembly empty block"
	};
	// print the results
	for(int i=0;i<loc;i++) {
		printf("results for [%s]\n",test_names[i]);
		printf("val_before is %d, val_after is %d\n",val_before[i],val_after[i]);
		if(val_before[i]==4000) {
			printf("compiler used register for value\n");
		} else {
			printf("compiler did not use register for value, you are probably running without optimization\n");
		}
		if(val_after[i]==2000) {
			printf("barrier worked\n");
		} else {
			printf("barrier did not work\n");
		}
	}
	return(0);
}
