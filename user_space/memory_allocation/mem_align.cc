#include <stdio.h>     // for printf,fprintf,perror
#include <unistd.h>    // for getpagesize,sysconf
#include <malloc.h>    // for valloc,memalign
#include <stdlib.h>    // for posix_memalign,malloc
#include <sys/mman.h>  // for mmap
#include <string.h>    // for memset

#include "us_helper.hh" // for printproc

/*
 *      This demo shows how to allocate memory which is PAGE_SIZE aligned...
 *
 *      To get the page size of the system two ways are presented:
 *      - getpagesize()
 *      - sysconf(_SC_PAGESIZE)
 *
 *      To get aligned memory 4 ways are presented:
 *      - valloc (deprecated)
 *      - memalign (deprecated)
 *      - posix_memalign (this is the one to use).
 *      - malloc (if you don't have any of the others...).
 *      - mmap anonymous pages (this is supposed to be quite effective...).
 *
 *              Mark Veltzer
 *
 * EXTRA_LIBS=
 */
void *mem_align(unsigned int size) {
	int ps = getpagesize();
	void *ptr;
	int res = posix_memalign(&ptr, ps, size);

	if (res != 0) {
		fprintf(stderr, "error with posix_memalign\n");
		exit(1);
	}
	return(ptr);
}


/*
 *      This function works by allocating more memory than is actually required.
 *      (One page more to be precise). Then asking malloc to allocate that memory
 *      and allocating the aligned block that we need within the block allocated
 *      by malloc...
 *
 *      NOTE: you should also keep a hash table to release the memory since free
 *      wants the ORIGINAL malloc returned pointer and not the rounded one. This
 *      is not demonstrated in this example...
 */
void *malloc_align(unsigned int size) {
	int ps = getpagesize();
	int pages = size / ps + 1;
	int new_size = pages * ps;
	void         *ptr = malloc(new_size);
	unsigned int iptr = (unsigned int)ptr;

	iptr = (iptr / ps + 1) * ps;
	return((void *)iptr);
}


/*
 *      mmap anonymous allocation function
 */
void *mmap_alloc(unsigned int size) {
	//int flags=MAP_ANONYMOUS | MAP_PRIVATE; /* we want anonymous mapping */
	int flags = MAP_ANONYMOUS | MAP_SHARED;                                                                                                                                                                              /* we want anonymous mapping */
	void *res = mmap(
	        NULL,                                                                                                                                                                                                                                                                   /* dont recommend address */
	        size,                                                                                                                                                                                                                                                                   /* the size we need */
	        PROT_READ | PROT_WRITE,                                                                                                                                                                                                                                                 /* we want read AND write */
	        flags,
	        -1,                                                                                                                                                                                                                                                                     /* we do not have a device to allocate from */
	        0                                                                                                                                                                                                                                                                       /* we dont need an offset as we don't have a file and are doing anon */
	        );

	if (res == MAP_FAILED) {
		perror("mmap failed");
		exit(1);
	}
	return(res);
}


int main(int argc, char **argv, char **envp) {
	// the size to allocate
	const unsigned int size = 20200;
	int ps1 = getpagesize();
	long ps2 = sysconf(_SC_PAGESIZE);
	int ps = ps1;

	printf("getpagesize() is %d\n", ps1);
	printf("sysconf(_SC_PAGESIZE) is %ld\n", ps2);
	void *pt1 = valloc(size);
	void *pt2 = memalign(ps, size);
	void *pt3 = mem_align(size);
	void *pt4 = malloc_align(size);
	void *pt5 = mmap_alloc(size);
	memset(pt1, 0, size);
	memset(pt2, 0, size);
	memset(pt3, 0, size);
	memset(pt4, 0, size);
	memset(pt5, 0, size);
	printf("pt1 is %p (%d)\n", pt1, (unsigned int)pt1 % ps);
	printf("pt2 is %p (%d)\n", pt2, (unsigned int)pt2 % ps);
	printf("pt3 is %p (%d)\n", pt3, (unsigned int)pt3 % ps);
	printf("pt4 is %p (%d)\n", pt4, (unsigned int)pt4 % ps);
	printf("pt5 is %p (%d)\n", pt5, (unsigned int)pt5 % ps);
	printproc(NULL);
	return(0);
}
