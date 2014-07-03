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

/*
 * This is a collection of helper function to help with working.
 */

/* THIS IS A C FILE, NO C++ here */

static inline unsigned int getrdtsc() {
	unsigned int val;
	asm ("rdtsc" : "=val" (val));
	return val;
}
static inline unsigned long getstackadr() {
#if __x86_64__
	return 0;
#else
	unsigned long val;
	asm ("movl %%esp, %0" : "=r" (val));
	return val;
#endif
}
static inline unsigned long page_align(unsigned long adr) {
	return (unsigned long)adr & (unsigned long)~(getpagesize()-1);
}
static inline void* getstackpointer() {
	return (void*)(page_align(getstackadr())+3*getpagesize());
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

#endif	/* !__lowlevel_utils_h */
