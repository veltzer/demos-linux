#include <iostream> // for std::cout, std::endl
#include <sys/mman.h> // for mmap(2), munmap(2)
#include <string.h> // for memset(3)

#include "us_helper.hh"

/*
 * This example creates lots of minor page faults...
 *
 *		Mark Veltzer
 */

void minor_fault_hog_function() {
	for(unsigned int i=0;i<1000000;i++) {
		int flags=MAP_PRIVATE | MAP_ANONYMOUS;
		const unsigned int size = 1024*1024;
		void* p;
		CHECK_NOT_NULL(p = mmap(NULL, size, PROT_READ | PROT_WRITE, flags, -1, 0));
		//memset(p, 0, size);
		for(unsigned int j=0;j<size;j+=2) {
			((char*)p)[j]=j;
		}
		CHECK_ZERO(munmap(p,size));
	}
}

int main(int argc, char **argv, char **envp) {
	minor_fault_hog_function();
	return EXIT_SUCCESS;
}
