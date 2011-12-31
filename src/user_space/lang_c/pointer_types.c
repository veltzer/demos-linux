#include <stdio.h>
#include <string.h>

/*
 *      This demos how to use compiler attributes to make sure we don't mix pointers that
 *      refer to different memory spaces...
 *
 *      Regular software developers usually do not use these features...
 *
 *              Mark Veltzer
 *
 * EXTRA_LIBS=
 */

// this is taken from the kernel sources (include/linux/compiler.h)
// currently this does not seem to work

//#define DO_IT

#ifdef DO_IT
#define __space1 __attribute__((noderef, address_space(1)))
#define __space2 __attribute__((noderef, address_space(2)))
#else // DO_IT
#define __space1
#define __space2
#endif // DO_IT
void func1(void *__space1 pointer) {
}


void *__space1 malloc1(unsigned int size) {
	return(NULL);
}


void func2(void *__space2 pointer) {
}


void *__space2 malloc2(unsigned int size) {
	return(NULL);
}


int main(int argc, char **argv, char **envp) {
	return(0);
}
