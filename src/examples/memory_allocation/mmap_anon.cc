#include <stdio.h> // for printf(3)
#include <string.h> // for memset(3)
#include <sys/mman.h> // for mmap(2)
#include <unistd.h> // for getpagesize(2)
#include <assert.h> // for assert(3)

#include "us_helper.hh"

/*
 *      This application demonstrates the use of anonymous memory mappings to get
 *      memory from the operating system.
 *      Notes:
 *      - memory returned from mmap is on a page boundry (see the assert).
 *      - you can set your own protection on the pages.
 *      - you can even set your own address instead of the null that we pass -
 *      	although there are issues with that.
 *      - you can use the memory like you would use any other.
 *      - if you want to avoid page faults here are three options:
 *      	- touch the memory (bzero, memset, ....).
 *      	- mlock
 *      	- MAP_POPULATE
 *      - later you can use this memory as shared memory with another process.
 *
 *              Mark Veltzer
 *
 * EXTRA_LIBS=-lproc
 */
// do you want to ask mmap to populate the page table ?
static bool do_populate=true;
// do you want to lock the memory after getting it ?
// you will need to run as sudo for this to work...
static bool do_mlock=false;
// do you want to touch the memory after getting it ?
static bool do_touch=false;

static inline void print_stats(void) {
	proc_t myproc;
	look_up_our_self(&myproc);
	printf("size is %ld, min_flt is %ld\n",myproc.rss, myproc.min_flt);
}

int main(int argc, char **argv, char **envp) {
	const int size = 1000000;
	void      *p;
	int flags=MAP_PRIVATE | MAP_ANONYMOUS;
	if(do_populate) {
		flags|=MAP_POPULATE;
	}

	printproc(NULL);
	print_stats();
	scp(p = mmap(NULL, size, PROT_READ | PROT_WRITE, flags, -1, 0));
	printproc(NULL);
	assert((unsigned int)p%getpagesize()==0);
	if(do_mlock) {
		// this next line needs permission to lock memory (check ulimit or run as sudo)
		sc(mlock(p,size));
	}
	if(do_touch) {
		memset(p, 0, size);
	}
	printf("p is %p\n", p);
	print_stats();
	return(0);
}
