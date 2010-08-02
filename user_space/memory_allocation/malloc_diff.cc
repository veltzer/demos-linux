#include <stdio.h>  // for printf(3)
#include <stdlib.h> // for malloc(3)
#include <assert.h> // for assert(3)

/*
 * This application just prints malloc diffs for various malloc allocation.
 * The idea is to use this application as a way to analyze how malloc allocates
 * memory...
 *
 *              Mark Veltzer
 *
 * EXTRA_LIBS=
 *
 */

int main(int argc, char **argv, char **envp) {
	void* prev=NULL;
	for(unsigned int i=0;i<100;i++) {
		unsigned int sizeToAlloc=i+1;
		void* p=malloc(sizeToAlloc);
		if(prev!=NULL) {
			unsigned int diff=(char*)p-(char*)prev;
			printf("allocated %d, diff is %d\n",sizeToAlloc,diff-sizeToAlloc);
		}
		prev=p;
	}
	return(0);
}
