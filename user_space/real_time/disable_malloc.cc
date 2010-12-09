/*
 * This program shows how you can disable malloc completely so that you will be sure
 * that in the running phase of your real-time application you are not using the malloc
 * functions which are sometimes not O(1) bound...
 *
 * This example uses malloc hooks in the C library to disable malloc.
 * Another way to achieve this is for you write your own malloc allocator to completely
 * replace malloc, or call malloc. Your allocator would provice the standard malloc
 * functions (malloc,realloc,memalign) and would throw exceptions or returns nulls
 * if these are called after a certain stage.
 *
 * 			Mark Veltzer
 */

#include <malloc.h> // for malloc(3) and the hooks
#include <assert.h> // for assert(3)
#include <iostream> // for std::exception

#include "us_helper.hh"

static bool malloc_allowed=true;

static void* my_malloc_hook(size_t size, const void *caller) {
	if(malloc_allowed) {
		return malloc(size);
	} else {
		throw std::exception();
	}
}

static void* my_realloc_hook(void *ptr, size_t size, const void *caller) {
	throw std::exception();
}

static void* my_memalign_hook(size_t alignment, size_t size,const void *caller) {
	throw std::exception();
}

static void my_free_hook(void *ptr, const void *caller) {
	throw std::exception();
}

/* Variables to save original hooks. */
static void *(*old_malloc_hook)(size_t, const void *);

static void my_init_hook(void) {
	TRACE("start");
	old_malloc_hook=__malloc_hook;
	__malloc_hook=my_malloc_hook;
	__realloc_hook=my_realloc_hook;
	__memalign_hook=my_memalign_hook;
	__free_hook=my_free_hook;
}

/* Override initializing hook from the C library. */
void (*__malloc_initialize_hook) (void) = my_init_hook;

void disable_malloc(void) {
	malloc_allowed=false;
}

void enable_malloc(void) {
	malloc_allowed=true;
}

int main(int argc,char** argv,char** envp) {
	// this allocation should succeed...
	void* p=malloc(200);
	assert(p!=NULL);
	// we are going into the critical phase of the application...
	disable_malloc();
	// this should fail...
	p=malloc(200);
	return 0;
}
