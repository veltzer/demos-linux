#include <stdio.h> // for printf(3)
/*
 * This example shows that two consecutive reads of the same value
 * are optimized away by the compiler.
 *
 * You can see this by building the disassembly file for this
 * example.
 *
 * Notice that the reads to the volatile are not optimized away
 * because the compiler treats the volatile with suspicion and
 * fears it may change value.
 *
 *              Mark Veltzer
 *
 * EXTRA_LIBS=
 */

void printit(int val);
void print_address_of(int* ptr);

int main(int argc, char **argv, char **envp) {
	volatile int i=6;
	int j=8;
	int a;
	// to make the compiler actually instantiate a and j
	print_address_of(&a);
	print_address_of(&j);
	a=i;
	a=i;
	printit(a);
	a=j;
	a=j;
	printit(a);
	return(0);
}

void print_address_of(int* ptr) {
	printf("ptr is %p\n",ptr);
}

void printit(int val) {
	printf("val is %d\n",val);
}
