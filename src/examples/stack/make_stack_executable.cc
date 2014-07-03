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
#include <stdio.h> // for printf(3)
#include <stdlib.h> // for EXIT_SUCCESS
#include <unistd.h> // for pause(2)
#include <sys/mman.h> // for mmap(2)
#include <err_utils.h> // for CHECK_NOT_M1()

int main(int argc, char** argv, char** envp) {
	/*
	void* p=mmap(NULL, 4096, PROT_EXEC | PROT_READ | PROT_WRITE , MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if(p==MAP_FAILED) {
		fprintf(stderr, "mmap failed");
		return EXIT_FAILURE;
	}
	*/
	const unsigned int num_pages=35;
	//void* p=get_stack_address();
	//unsigned int stack_size=140*4096;
	void* o=&argc;
	printf("o is at %p\n",o);
	unsigned int p_int=(unsigned int)&argc & (unsigned int)~4095;
	p_int+=4*4096;
	p_int-=num_pages*4096;
	void* p=(void*)p_int;
	printf("stack is at %p\n",p);
	CHECK_NOT_M1(mprotect(p, num_pages*4096, PROT_READ | PROT_WRITE | PROT_EXEC));
	while(true) {
		pause();
		printf("hey, I got a signal!\n");
	}
	return EXIT_SUCCESS;
}
