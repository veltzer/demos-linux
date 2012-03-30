#include <stdlib.h> // for malloc(3)
#include <assert.h> // for assert(3)

#include "us_helper.hh"

/*
 * This application just prints malloc diffs for various malloc allocation.
 * The idea is to use this application as a way to analyze how malloc allocates
 * memory...
 *
 *		Mark Veltzer
 */

/*
 * This function guesses the next address that malloc(3) will allocate to your
 * code.
 *
 * This is the basic stuff:
 * - malloc needs at least 16 bytes per chunk.
 * - it also needs at least 3 bytes for it's own internal data.
 * - it will allocate on an 8 byte boundary.
 */

void* guess_next_address(void* prev,unsigned int size) {
	if(size<8) {
		size+=8;
	} else {
		if(size%8>5) {
			size+=8;
		}
	}
	size+=8-size%8;
	char* cprev=(char*)prev;
	cprev+=size;
	return cprev;
}

int main(int argc, char **argv, char **envp) {
	void* prev=NULL;
	for(unsigned int sizeToAlloc=1;sizeToAlloc<100;sizeToAlloc++) {
		void* guess=NULL;
		if(prev!=NULL) {
			guess=guess_next_address(prev,sizeToAlloc);
		}
		void* p=malloc(sizeToAlloc);
		if(prev!=NULL) {
			//TRACE("guess is %p, p is %p, sizeToAlloc is %d",guess,p,sizeToAlloc);
			assert(guess==p);
			// this is debug code aimed at understanding how the malloc
			// engine works...
			//unsigned int diff=(char*)p-(char*)prev;
			//TRACE("allocated %d, real diff is %d, diff is %d, diff mod 8 is %d\n",sizeToAlloc,diff, diff-sizeToAlloc , diff%8);
		}
		prev=p;
	}
	return EXIT_SUCCESS;
}
