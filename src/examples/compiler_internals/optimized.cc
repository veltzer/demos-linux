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
#include <stdio.h>	// for printf(3), fopen(3)
#include <stdlib.h>	// for EXIT_SUCCESS
#include <us_helper.h>	// for my_system()

/*
 * This example shows that the compiler is very aggresive on optimization.
 * It doesn't even unroll the loop! It simply calculates (1+99)*99/2=4950
 * and puts the value in the code.
 * Conclusion: DO NOT UNDERESTIMATE THE COMPILER!
 *
 * this is to make sure that source code interleaving in the disassembly works
 * out well...
 * EXTRA_COMPILE_FLAGS=-g3
 */

FILE* out;
// first lets try a loop without optimization
#pragma GCC push_options
#pragma GCC optimize ("O0")
void f1() __attribute__((noinline));
void f1() {
	unsigned int sum1=0;
	for(unsigned int i=0; i<100; i++) {
		sum1+=i;
	}
	fprintf(out, "sum1 is %u\n", sum1);
}
#pragma GCC pop_options

int main(int argc, char** argv, char** envp) {
	out=fopen("/dev/null", "w");
	f1();
	// now with optimization
	unsigned int sum2=0;
	for(unsigned int i=0; i<100; i++) {
		sum2+=i;
	}
	fprintf(out, "sum2 is %u\n", sum2);
	// now with printing
	unsigned int sum3=0;
	for(unsigned int i=0; i<100; i++) {
		fprintf(out, "i is %d\n", i);
		sum3+=i;
	}
	fprintf(out, "sum3 is %u\n", sum3);
	my_system("objdump --disassemble --source %s", argv[0]);
	return EXIT_SUCCESS;
}
