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
#include <unistd.h>	// for getpagesize(2), sysconf(3)
#include <malloc.h>	// for valloc(3), memalign(3)
#include <stdlib.h>	// for posix_memalign(3) , malloc(3), EXIT_SUCCESS
#include <sys/mman.h>	// for mmap(2)
#include <string.h>	// for memset(3)
#include <us_helper.h>	// for printproc(), CHECK_ZERO(), CHECK_NOT_VOIDP()

/*
 * This demo shows how to allocate memory which is PAGE_SIZE aligned...
 *
 * To get the page size of the system two ways are presented:
 * - getpagesize()
 * - sysconf(_SC_PAGESIZE)
 *
 * To get aligned memory 4 ways are presented:
 * - valloc (deprecated)
 * - memalign (deprecated)
 * - posix_memalign (this is the one to use).
 * - malloc (if you don't have any of the others...).
 * - mmap anonymous pages (this is supposed to be quite effective...).
 */

// this next function takes an address and aligns it to page size
inline void* align_address(void* addr) {
	unsigned long uladdr=(unsigned long)addr;
	int ps=getpagesize();
	return (void*) (uladdr & ~(ps-1));
}

void *mem_align(unsigned int size) {
	int ps=getpagesize();
	void *ptr;
	CHECK_ZERO(posix_memalign(&ptr, ps, size));
	return(ptr);
}

/*
 * This function works by allocating more memory than is actually required.
 * (One page more to be precise). Then asking malloc to allocate that memory
 * and allocating the aligned block that we need within the block allocated
 * by malloc...
 *
 * NOTE: you should also keep a hash table to release the memory since free
 * wants the ORIGINAL malloc returned pointer and not the rounded one. This
 * is not demonstrated in this example...
 */
void *malloc_align(unsigned int size) {
	int ps=getpagesize();
	int pages=size / ps + 1;
	int new_size=pages * ps;
	void* ptr=malloc(new_size);
	unsigned long iptr=(unsigned long)ptr;

	iptr=(iptr / ps + 1) * ps;
	return((void *)iptr);
}

/*
 * mmap anonymous allocation function
 */
void *mmap_alloc(unsigned int size) {
	/* we want anonymous mapping */
	int flags=MAP_ANONYMOUS;
	flags|=MAP_PRIVATE;
	// flags|=MAP_SHARED;
	void* res=CHECK_NOT_VOIDP(mmap(
			NULL,	/* dont recommend address */
			size,	/* the size we need */
			PROT_READ | PROT_WRITE,	/* we want read AND write */
			flags,
			-1,	/* we do not have a device or fd to allocate from */
			0	/* we dont need an offset as we don't have a file and are doing anon */
			), MAP_FAILED);
	return(res);
}

int main(int argc, char** argv, char** envp) {
	// the size to allocate
	const unsigned int size=20200;
	int ps1=getpagesize();
	long ps2=sysconf(_SC_PAGESIZE);
	int ps=ps1;
	printf("getpagesize() is %d\n", ps1);
	printf("sysconf(_SC_PAGESIZE) is %ld\n", ps2);
	void *pt1=valloc(size);
	void *pt2=memalign(ps, size);
	void *pt3=mem_align(size);
	void *pt4=malloc_align(size);
	void *pt5=mmap_alloc(size);
	memset(pt1, 0, size);
	memset(pt2, 0, size);
	memset(pt3, 0, size);
	memset(pt4, 0, size);
	memset(pt5, 0, size);
	printf("pt1 is %p (%ld)\n", pt1, (unsigned long)pt1 % ps);
	printf("pt2 is %p (%ld)\n", pt2, (unsigned long)pt2 % ps);
	printf("pt3 is %p (%ld)\n", pt3, (unsigned long)pt3 % ps);
	printf("pt4 is %p (%ld)\n", pt4, (unsigned long)pt4 % ps);
	printf("pt5 is %p (%ld)\n", pt5, (unsigned long)pt5 % ps);
	printproc(NULL);
	void* ptr=(void*)main;
	printf("ptr is %p\n", ptr);
	printf("ptr aligned is %p\n", align_address(ptr));
	return EXIT_SUCCESS;
}
