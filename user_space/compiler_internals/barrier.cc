#include <stdio.h> // for printf(3)
#include <stdlib.h> // for srandom(3)

/*
 *	This is an example of a memory barrier
 *	This example needs to be compiled with optimizations to see it in action.
 *	View the resulting assembly code with:
 *	objdump --disassemble --line-numbers --source --demangle barrier
 *
 *	The gcc compiler barrier
 *	is __sync_synchronize(). It states it is a *FULL* barrier which means
 *	that it is also a machine barrier. The aseembly code should tell us
 *	that quite clearly.
 *	It seems that gcc does not have the notion of a *read* vs *write*
 *	barrier which could come in handly instead of this dominating
 *	*FULL* barrier...
 *
 *	Machine memory barriers
 *	There does not seem to be a user space equivalent wrapper of gcc to the
 *	kernel rmb(), wmb(), mb() and the smp equivalents for machine memory
 *	barriers.. I still haven't figured out if this is due to the machine
 *	instructions themselves being prohibited in user space or just a lack
 *	of gcc wrappers for them and therefore the need to code them in inline
 *	assembly.
 *
 *	TODO:
 *	make this program show the instructions that are emitted for main so
 *	that people could see the assembly code generated.
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
	
	// FOURTH EXAMPLE (inline assembly)
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

	// print the results
	for(int i=0;i<loc;i++) {
		printf("val_before is %d\n",val_before[i]);
		printf("val_after is %d\n",val_after[i]);
	}
	return(0);
}
