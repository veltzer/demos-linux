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
static inline unsigned int getstackpointer() {
	unsigned int val;
	asm ("movl %%esp, %0" : "=r" (val));
	return val;
}
static inline unsigned int getframepointer() {
	unsigned int val;
	asm ("movl %%ebp, %0" : "=r" (val));
	return val;
}

int main(int argc, char** argv, char** envp) {
	volatile int a=6;
	printf("&a is %p\n", &a);
	printf("stackpointer is %p\n", (void*)getstackpointer());
	printf("framepointer is %p\n", (void*)getframepointer());
	printf("rdtsc is %u\n", getrdtsc());
	return EXIT_SUCCESS;
}
