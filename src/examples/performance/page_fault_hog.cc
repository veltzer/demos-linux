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
#include <iostream>	// for std::cout, std::endl
#include <sys/mman.h>	// for mmap(2), munmap(2)
#include <string.h>	// for memset(3)
#include <us_helper.h>	// for CHECK_NOT_NULL()

/*
 * This example creates lots of minor page faults...
 */

void minor_fault_hog_function() {
	for(unsigned int i=0; i<1000000; i++) {
		int flags=MAP_PRIVATE | MAP_ANONYMOUS;
		const unsigned int size=1024*1024;
		void* p=CHECK_NOT_NULL(mmap(NULL, size, PROT_READ | PROT_WRITE, flags, -1, 0));
		// memset(p, 0, size);
		for(unsigned int j=0; j<size; j+=2) {
			((char*)p)[j]=j;
		}
		CHECK_ZERO(munmap(p, size));
	}
}

int main(int argc, char** argv, char** envp) {
	minor_fault_hog_function();
	return EXIT_SUCCESS;
}
