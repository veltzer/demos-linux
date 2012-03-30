#include<stdio.h> // for printf(3)
#include<stdlib.h> // for malloc(3)
#include<sys/time.h> // for gettimeofday(2)
#include<obstack.h> // for obstack_*(3)

#include"us_helper.hh" // for my_system

/*
 * This example shows how to use obstacks...
 *
 * Notes:
 * - the obstack API is C so it can be used in both C and C++ apps.
 * - notice the speed of obstack compared to the speed of malloc.
 * 	because this is an uncontended case with just a single thread
 * 	then malloc and obstack perform roughly the same (malloc
 * 	on occasion a little slower...)
 *
 * TODO:
 * - add multi threading and releases to really show the difference
 * between obstacks and malloc.
 *
 *	Mark Veltzer
 */

// these are needed so that obstacks would work fine...
#define obstack_chunk_alloc xmalloc
#define obstack_chunk_free free
void* xmalloc(size_t size) {
	register void *value;
	CHECK_NOT_NULL(value=malloc(size));
	return value;
}

int main(int argc,char** argv,char** envp) {
	static struct obstack myobstack;
	obstack_init(&myobstack);
	struct timeval t1, t2;
	const unsigned int loop=100000;
	const unsigned int size_to_alloc=100;

	printf("testing obstack_alloc...\n");
	gettimeofday(&t1, NULL);
	for(unsigned int i=0;i<loop;i++) {
		obstack_alloc(&myobstack,size_to_alloc);
	}
	gettimeofday(&t2, NULL);
	printf("time in micro of one call: %lf\n", micro_diff(&t1,&t2)/(double)loop);

	printf("testing malloc...\n");
	gettimeofday(&t1, NULL);
	for(unsigned int i=0;i<loop;i++) {
		void* p __attribute__((unused))=malloc(size_to_alloc);
	}
	gettimeofday(&t2, NULL);
	printf("time in micro of one call: %lf\n", micro_diff(&t1,&t2)/(double)loop);
	return EXIT_SUCCESS;
}
