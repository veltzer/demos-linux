#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>

int main(int argc, char** argv, char** envp) {
	// lets allocate a buffer of 1GB
	const size_t size_to_alloc=1024*1024*1024;
	const unsigned int number_of_pages_to_touch=100;
	char* buf=(char*)malloc(size_to_alloc);
	mlockall(MCL_CURRENT|MCL_FUTURE);
	//madvise(buf, size_to_alloc, MADV_RANDOM);
	//memset(buf, 0, size_to_alloc);
	/*
	char* vbuf=buf;
	size_t size=size_to_alloc;
	unsigned long sum=0;
	while(size>0) {
		*vbuf=0;
		printf("vbuf is %p\n", vbuf);
		vbuf+=4096;
		size-=4096;
	}
	printf("sum is %lu\n", sum);
	*/
	unsigned int counter=0;
	while(true) {
		// touch some memory
		for(unsigned int i=0;i<number_of_pages_to_touch;i++) {
			*buf=0;
			buf+=4096;
		}
		counter++;
		printf("I touched some memory, %d\n", counter);
		// sleep
		sleep(1);
	}
	return EXIT_SUCCESS;
}
