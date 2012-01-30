#include <stdio.h> // for printf(3)
#include <stdlib.h> // for exit(3), malloc(3)
#include <sys/time.h> // for setrlimit(2)
#include <sys/resource.h> // for setrlimit(2)

#include "us_helper.hh"

/*
 * This example explores the use of limits. You can limit the size of your programs RAM
 * and crash long before you get into swapping (recommended).
 * Many more limits exist. Try to use as much of them as possible.
 * set setrlimit(2) for more details.
 *
 * You can ofcourse skip the setting of ulimit in the source code and use ulimit(1) on the
 * command line instead.
 *
 * 				Mark Veltzer
 */

int main(int argc,char** argv,char** envp) {
	const int max_megs=50;
	struct rlimit rlim;
	rlim.rlim_max=max_megs*1024*1024;
	rlim.rlim_cur=max_megs*1024*1024;
	sc(setrlimit(RLIMIT_AS,&rlim));
	int d=0;
	while(true) {
		void* m=malloc(1024*1024);
		if(m==NULL) {
			printf("Oh oh, no memory for me\n");
			exit(1);
		}
		printf("managed to allocate %d megs\n",d);
		d++;
	}
	return(0);
}
