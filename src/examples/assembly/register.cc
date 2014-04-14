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

#include <firstinclude.h>
#include <stdio.h>	// for printf(3)
#include <stdlib.h>	// for EXIT_SUCCESS

/*
 * This is an example of getting a register on an i32 machine
 */

static inline unsigned int getrdtsc() {
	unsigned int val;
	asm ("rdtsc" : "=val" (val));
	return val;
}
static inline unsigned long getstackpointer() {
#if __x86_64__
	return 0;
#else
	unsigned long val;
	asm ("movl %%esp, %0" : "=r" (val));
	return val;
#endif
}
static inline unsigned long getframepointer() {
#if __x86_64__
	return 0;
#else
	unsigned long val;
	asm ("movl %%ebp, %0" : "=r" (val));
	return val;
#endif
}

int main(int argc, char** argv, char** envp) {
	volatile int a=6;
	printf("&a is %p\n", &a);
	printf("stackpointer is %p\n", (void*)getstackpointer());
	printf("framepointer is %p\n", (void*)getframepointer());
	printf("rdtsc is %u\n", getrdtsc());
	return EXIT_SUCCESS;
}
