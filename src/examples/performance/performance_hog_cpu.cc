/*
        This file is part of the linuxapi project.
        Copyright (C) 2011-2013 Mark Veltzer <mark.veltzer@gmail.com>

        The linuxapi package is free software; you can redistribute it and/or
        modify it under the terms of the GNU Lesser General Public
        License as published by the Free Software Foundation; either
        version 2.1 of the License, or (at your option) any later version.

        The linuxapi package is distributed in the hope that it will be useful,
        but WITHOUT ANY WARRANTY; without even the implied warranty of
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
        Lesser General Public License for more details.

        You should have received a copy of the GNU Lesser General Public
        License along with the GNU C Library; if not, write to the Free
        Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
        02111-1307 USA.
 */

#include <firstinclude.h>
#include <stdlib.h>	// for EXIT_SUCCESS

/*
 * This is a simple example that spends lots of time in a function.
 * You can watch this with 'sudo perf top' and enter the function with 's'.
 *
 * make sure you compile this with debug info if you want to see
 * perf annotate the machine code with your code.
 *
 * Notes:
 * - you need the sudo since perf works by hardware events and support for them
 *in the kernel.
 * - you can avoid root with:
 *	Consider tweaking /proc/sys/kernel/perf_event_paranoid:
 *	-1 - Not paranoid at all
 *	0 - Disallow raw tracepoint access for unpriv
 *	1 - Disallow cpu events for unpriv
 *	2 - Disallow kernel profiling for unpriv
 * - if you strip the executable you will not get the function names.
 *
 * This is to make sure I can see annotation.
 * EXTRA_COMPILE_FLAGS=-g3
 */

class A {
public:
	// the function is not inlined on purpose so it would show up in
	//profilers
	// like 'perf(1)'...
	// static void performance_hog_function2() {
	static void performance_hog_function_cpu() __attribute__((noinline)) {
		float sum=0;
		for(unsigned int i=0; i<1000000; i++) {
			for(unsigned int j=0; j<500000; j++) {
				sum+=i*j;
			}
			A::inner_loop(&sum, i);
		}
	}
	static void inner_loop(float* sum, int i) __attribute__((noinline)) {
		for(unsigned int j=0; j<1000000; j++) {
			*sum+=i*j;
		}
	}
};

int main(int argc, char** argv, char** envp) {
	A::performance_hog_function_cpu();
	return EXIT_SUCCESS;
}
