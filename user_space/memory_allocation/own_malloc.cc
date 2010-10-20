#include<stdlib.h> // for malloc(3), calloc(3), realloc(3), free(3)

/*
 * This example shows how to correctly define your own malloc functions so that gcc will
 * work correctly with them.
 *
 * The use of the alloc_size attribute allows the compiled built in functions to know
 * the size of the object and to know that the pointer returned by the functions is a new
 * pointer and therefore will not be equal to any pointer seen the in the past (how does this
 * work with realloc ?!? - need to check this out...).
 *
 * 	Mark Veltzer
 */

void* my_calloc(size_t, size_t) __attribute__((alloc_size(1,2)));
void* my_realloc(void*,size_t) __attribute__((alloc_size(2)));
void* my_malloc(size_t) __attribute__((alloc_size(1)));
void my_free(void*); // no need for attributes of free

void* my_calloc(size_t nmemb, size_t size) {
	return calloc(nmemb,size);
}

void* my_realloc(void* ptr,size_t size) {
	return realloc(ptr,size);
}

void* my_malloc(size_t size) {
	return malloc(size);
}

void my_free(void* ptr) {
	free(ptr);
}

int main(int argc,char** argv,char** envp) {
	return(0);
}
