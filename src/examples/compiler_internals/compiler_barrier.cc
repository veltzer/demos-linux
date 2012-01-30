#include <stdio.h> // for printf(3)
#include <stdlib.h> // for srandom(3)

/*
 *	This is an example of a compiler barrier
 *	This example needs to be compiled with optimizations to see it in action.
 *	View the resulting assembly code with:
 *	objdump --disassemble --line-numbers --source --demangle barrier
 *
 *	The gcc compiler barrier
 *	========================
 *	is the asm(memory) one. It is not a CPU reordering barrier - just a compiler
 *	one.
 *	It seems that gcc does not have the notion of a *read* vs *write*
 *	barrier which could come in handly instead of this dominating
 *	*FULL* barrier.
 *
 *	Function call barriers
 *	======================
 *	On some platforms/compilers these are also memory barriers but no so in gcc
 *	that assumes that functions that are called do not change the content of registers
 *	(and if they do, they put back everything the way they found it...). In any case
 *	you cannot really rely on them since you are not sure which functions are macros,
 *	inlines etc. Better to use an official compiler barrier. I picked a function that
 *	is definately not an inlined one and still this it does not perform well as
 *	a compiler barrier (srandom on gcc 4.5.2).
 *
 *	Machine memory barriers
 *	=======================
 *	is the __sync_synchronize() one. There doesn't seem to be a read vs write
 *	one. This does not serve as a compiler barrier as is evident as a result
 *	of this program.
 *
 *	Empty assembly block
 *	====================
 *	Used to work in some versions of gcc but stopped working. Better not to use.
 *
 *	References:
 *	http://ridiculousfish.com/blog/archives/2007/02/17/barrier/
 *
 *	TODO:
 *	- make this program show the instructions that are emitted for main so
 *	that people could see the assembly code generated.
 *
 *		Mark Veltzer
 */

int main(int argc, char **argv, char **envp) {
	int a = 0;
	int u = 0;
	const int max=10;
	const int CORRECT_VAL=2000;
	const int WRONG_VAL=4000;
	int val_before[max];
	int val_after[max];
	const char* tests[max];
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

	// this should give you the same values, which are the WRONG values.
	tests[loc]="doing nothing";
	a=1000;
	//this code will cause the compiler to put a into a register
	while (a < 3000) {
		a+=a;
	}
	// the compiler writes a to it's 'natural' place on the stack
	*p = CORRECT_VAL;
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

	// This is a MACHINE memory barrier and not a compiler barrier. It does not
	// act as a compiler barrier but. This does not work.
	tests[loc]="__sync_synchronize()";
	a=1000;
	while (a < 3000) {
		a+=a;
	}
	*p = CORRECT_VAL;
	val_before[loc]=a;
	// the next macro actually inlines a machine instruction...
	__sync_synchronize();
	val_after[loc]=a;
	loc++;
	
	// this works on some compilers but not on others
	// on gcc 4.5.2 it does not and in general expect more modern compiler
	// to grow smarter and thus see that you are not really doing anything
	// in between...
	tests[loc]="empty assembly block";
	a=1000;
	while (a < 3000) {
		a+=a;
	}
	*p = CORRECT_VAL;
	val_before[loc]=a;
	// an empty asm statement to see if the compiler creates a barrier over it
	asm volatile("");
	val_after[loc]=a;
	loc++;


	// this works because a function is a natural compiler barrier
	tests[loc]="function call barrier";
	a=1000;
	while (a < 3000) {
		a+=a;
	}
	*p = CORRECT_VAL;
	val_before[loc]=a;
	// a function call is a natural compiler barrier, but not so in gcc
	// which is very aggressive on optimization...
	srandom(100);
	val_after[loc]=a;
	loc++;
	
	// this works because it is the official compiler barrier for gcc
	tests[loc]="official compiler barrier";
	a=1000;
	while (a < 3000) {
		a+=a;
	}
	*p = CORRECT_VAL;
	val_before[loc]=a;
	// a statement telling the GNU assembler not to reorder around it
	asm volatile("" ::: "memory");
	val_after[loc]=a;
	loc++;

	// print the results
	printf("=============================================================\n");
	for(int i=0;i<loc;i++) {
		printf("results for [%s]\n",tests[i]);
		printf("val_before is %d, val_after is %d\n",val_before[i],val_after[i]);
		if(val_before[i]==WRONG_VAL) {
			printf("compiler used register for value\n");
		} else {
			printf("compiler did not use register for value, you are probably running without optimization\n");
		}
		if(val_after[i]==CORRECT_VAL) {
			printf("barrier worked\n");
		} else {
			printf("barrier did not work\n");
		}
		printf("=============================================================\n");
	}
	return(0);
}
