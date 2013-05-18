/*
 * This file is part of the linuxapi project.
 * Copyright (C) 2011-2013 Mark Veltzer <mark.veltzer@gmail.com>
 *
 * The linuxapi package is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * The linuxapi package is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with the GNU C Library; if not, write to the Free
 * Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 * 02111-1307 USA.
 */

#include <firstinclude.h>
#include <stdio.h>	// for printf(3), fprintf(3)
#include <stdlib.h>	// for srandom(3), exit(3), EXIT_SUCCESS, EXIT_FAILURE
#include <us_helper.h>	// for CHECK_NOT_NULL()

/*
 * This is an example of a compiler barrier
 * This example needs to be compiled with optimizations to see it in action.
 * View the resulting assembly code with:
 * objdump --disassemble --line-numbers --source --demangle barrier
 *
 * The gcc compiler barrier
 * ========================
 * is the asm(memory) one. It is not a CPU reordering barrier - just a compiler
 * one.
 * It seems that gcc does not have the notion of a *read* vs *write*
 * barrier which could come in handly instead of this dominating
 * *FULL* barrier.
 *
 * Function call barriers
 * ======================
 * On some platforms/compilers these are also memory barriers but not so in gcc
 * that assumes that functions that are called do not change the content of registers
 * (and if they do, they put back everything the way they found it...). In any case
 * you cannot really rely on them since you are not sure which functions are macros,
 * inlines etc. Better to use an official compiler barrier. I picked a function that
 * is definately not an inlined one and still this it does not perform well as
 * a compiler barrier (srandom on gcc 4.5.2).
 *
 * Machine memory barriers
 * =======================
 * is the __sync_synchronize() one. There doesn't seem to be a read vs write
 * one. This does not serve as a compiler barrier as is evident as a result
 * of this program.
 *
 * Empty assembly block
 * ====================
 * Used to work in some versions of gcc but stopped working. Better not to use.
 *
 * Notes:
 * - the loop that you see in the code forces the compiler to use a register
 * to hold the 'a' variable (if you compile with optimisation ofcourse).
 * - if you disassemble the code you see that the compiler writes the loop variable
 * (a in our case) back to it's natural place on the stack right after the loop.
 * - after the loop suprisingly, the compiler is certain that the register is still
 * holding the right value for a and so uses it in the print...
 * - without a good compiler barrier the compiler is certain that the value in the
 * register is synchronized with the memory location.
 *
 * References:
 * http://ridiculousfish.com/blog/archives/2007/02/17/barrier/
 *
 * TODO:
 * - make this program show the instructions that are emitted for main so
 * that people could see the assembly code generated.
 */

// a small function to tell you if the stack direction is up or down...
bool stack_direction_up() __attribute__((noinline));
bool stack_direction_up() {
	int a;
	int u;
	unsigned long pa=(unsigned long)&a;
	unsigned long pu=(unsigned long)&u;
	if(pa==pu+sizeof(int)) {
		return false;
	}
	if(pa==pu-sizeof(int)) {
		return true;
	}
	fprintf(stderr, "strange stack you have here...\n");
	exit(EXIT_FAILURE);
}

FILE* outfile;

#define TEST(shortname, desc, code) \
	void test_ ## shortname() __attribute__((noinline)); \
	void test_ ## shortname(volatile int& val_before, volatile int& val_after, int& dummy) { \
		int a=0; \
		int u=0; \
		const int CORRECT_VAL=2000; \
		const int WRONG_VAL=3200; \
		/* p will point to a but the compiler does not know it.*/ \
		int* p=&u; \
		if(stack_direction_up()) { \
			p--; \
		} else { \
			p++; \
		} \
		/* this taking of the address of a to have the compiler actually store
		 * a on the stack at all! If we don't do this the compiler will treat
		 * a as a register for the entire scope of this function! */ \
		int *pa=&a; \
		/* this printing is essential to keep the compiler from telling us
		 * that 'pa' is an unused variable... */ \
		fprintf(outfile, "pa is %p\n", pa); \
		fprintf(outfile, "p is %p\n", p); \
		fprintf(outfile, "&a is %p\n", &a); \
		fprintf(outfile, "now starting...\n"); \
		a=100; \
		while(a < 3000) { \
			a+=a; \
		} \
		/*asm("nop");*/	\
		*p=CORRECT_VAL;	\
		val_before=a; \
		code; \
		val_after=a; \
		/*asm("nop");*/	\
		printf("results for [%s]\n", __stringify(code)); \
		printf("description [%s]\n", desc); \
		printf("val_before is %d, val_after is %d\n", val_before, val_after); \
		if(val_before==WRONG_VAL) { \
			printf("compiler used register for value\n"); \
		} else { \
			printf("compiler did not use register for value, you are probably running without optimization\n"); \
		} \
		if(val_after==CORRECT_VAL) { \
			printf("barrier worked\n"); \
		} else { \
			printf("barrier did not work\n"); \
		} \
		printf("===========================================\n"); \
	}

TEST(
	nothing,
	"doing nothing. this should give you the same values, which are the WRONG values.",
	);
TEST(
	machbar,
	"This is a full barrier which is both a MACHINE memory barrier\
	and a compiler barrier. It should work.\
	the next macro actually inlines a machine instruction...",
	__sync_synchronize()
	);
TEST(
	emptasm,
	"empty assembly block which works on some compilers but not on others \
	on gcc 4.5.2 it does not and in general expect more modern compiler \
	to grow smarter and thus see that you are not really doing anything \
	in between...",
	asm volatile ("")
	);
TEST(
	funccall,
	"calling extern function works on some compilers and some functions\
	because a function is a natural compiler barrier\
	but watch out for functions annotated with certain annotations\
	or inlined functions",
	srandom(5)
	);
TEST(
	fullcompbar,
	"official compiler barrier for gcc which is\
	a statement telling the GNU assembler not to reorder around it",
	asm volatile ("" ::: "memory")
	);
TEST(
	singvarbar,
	"a single variable barrier -> this one works",
	asm volatile ("" : "=g" (a) ::)
	);
TEST(
	singvarbar2,
	"a single variable barrier -> this one doesnt (bahaha for the people on stackoverflow...)",
	asm volatile ("" : "=r" (a) ::)
	);
TEST(
	singvarbar3,
	"a single variable barrier -> works (surprisingly!)",
	asm volatile ("" : "=r" (dummy) ::)
	);
TEST(
	singvarvol,
	"attempt to use volatile on left side to barrier the compiler (this one doesnt work)",
	*(volatile int *)&a=a;
	);
TEST(
	singvarvolright,
	"attempt to use volatile on right side to barrier the compiler (this one does work)",
	a=*(volatile int*)&a;
	);
TEST(
	singvarvol2,
	"attempt to use volatile to barrier the compiler",
	{ int y=a; *(volatile int*)&a=y; }
	);
TEST(
	atomicop,
	"attempt to do an atomic operation on the var",
	__sync_bool_compare_and_swap(&a, 0, 0)
	);

int main(int argc, char** argv, char** envp) {
	outfile=(FILE*)CHECK_NOT_NULL(fopen("/dev/null", "w"));
	int val_before, val_after, dummy;
	test_nothing(val_before, val_after, dummy);
	test_machbar(val_before, val_after, dummy);
	test_emptasm(val_before, val_after, dummy);
	test_funccall(val_before, val_after, dummy);
	test_fullcompbar(val_before, val_after, dummy);
	test_singvarbar(val_before, val_after, dummy);
	test_singvarbar2(val_before, val_after, dummy);
	test_singvarbar3(val_before, val_after, dummy);
	test_singvarvol(val_before, val_after, dummy);
	test_singvarvolright(val_before, val_after, dummy);
	test_singvarvol2(val_before, val_after, dummy);
	test_atomicop(val_before, val_after, dummy);
	return EXIT_SUCCESS;
}
