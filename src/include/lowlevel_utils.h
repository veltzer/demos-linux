/*
 * This file is part of the linuxapi package.
 * Copyright (C) 2011-2014 Mark Veltzer <mark.veltzer@gmail.com>
 *
 * linuxapi is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * linuxapi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with linuxapi. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __lowlevel_utils_h
#define __lowlevel_utils_h

#include <firstinclude.h>
#include <unistd.h> // for getpagesize(2)
#include <stdio.h> // for fprintf(3), stderr(O)
#include <stdlib.h> // for EXIT_FAILURE, exit(3)

/*
 * This is a collection of helper function to help with working.
 */

/* THIS IS A C FILE, NO C++ here */

/*
 * a small function to tell you if the local variables addresses
 * on the stack are growing or diminishing with order of declaration
 */
bool stack_vars_direction_up() __attribute__((noinline));
bool stack_vars_direction_up() {
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

/*
 * a small function to tell you in which direction your stack grows
 * with regard to function call.
 */
unsigned long stack_function_direction_up_inner() __attribute__((noinline));
unsigned long stack_function_direction_up_inner() {
	int a;
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wreturn-local-addr"
	return (unsigned long)&a;
	#pragma GCC diagnostic pop
}

bool stack_function_direction_up() __attribute__((noinline));
bool stack_function_direction_up() {
	int a;
	unsigned long pa=(unsigned long)&a;
	unsigned long adr=stack_function_direction_up_inner();
	if(adr>pa) {
		return true;
	}
	if(adr<pa) {
		return false;
	}
	fprintf(stderr, "strange stack you have here...\n");
	exit(EXIT_FAILURE);
}

/*
 * align an address down to page boundry
 */
static inline unsigned long page_align(unsigned long adr) {
	return adr & ~((unsigned long)getpagesize()-1);
}

/*
 * align an address to stack (either up or down)
 */
static inline unsigned long stack_align(unsigned long adr) {
	if(stack_function_direction_up()) {
		return adr & ~((unsigned long)getpagesize()-1);
	} else {
		unsigned long mod=adr%getpagesize();
		if(mod==0) {
			return adr;
		} else {
			return adr-mod+getpagesize();
		}
	}
}
/*
 * align a pointer to the stack (either up or down)
 */
static inline void* stack_align_pointer(void* p) {
	return (void*)stack_align((unsigned long)p);
}

/*
 * get the RDTSC register (counter)
 * This functions seems to be wrong since rdtsc should be 64 bit wide register.
 */
static inline unsigned int getrdtsc() {
	unsigned int val;
	asm ("rdtsc" : "=val" (val));
	return val;
}

/*
 * get the stack pointer register
 * on a 32 bit Intel machine this already works. I need to make it work on x64.
 */
static inline unsigned long getstackadr() {
#if __x86_64__
	return 0;
#else
	unsigned long val;
	asm ("movl %%esp, %0" : "=r" (val));
	return val;
#endif
}

/*
 * get the stack pointer as address
 */
static inline void* getstackpointer() {
	return (void*)getstackadr();
}

/*
 * get the current frame pointer
 * This currently works only on i386
 */
static inline unsigned long getframepointer() {
#if __x86_64__
	return 0;
#else
	unsigned long val;
	asm ("movl %%ebp, %0" : "=r" (val));
	return val;
#endif
}

#endif	/* !__lowlevel_utils_h */
