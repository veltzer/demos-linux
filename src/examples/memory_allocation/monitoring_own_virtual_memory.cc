#include <stdlib.h> // for malloc(3)
#include <stdio.h> // for printf(3)
#include <sys/time.h> // for getrusage(2)
#include <sys/resource.h> // for getrusage(2)
#include <strings.h> // for bzero(3)

#include "us_helper.hh"

/*
 * This is an example which shows how to monitor your own virtual memory using getrusage
 *
 * Note that if you do not zero the memory (see the bzero call below) then this program
 * works much faster and does not generate and page faults and the usage stays the same.
 *
 * 			Mark Veltzer
 */

extern char etext, edata, end; /* The symbols must have some type, or "gcc -Wall" complains */

void show_vmem() {
	struct rusage usage;
	CHECK_NOT_M1(getrusage(RUSAGE_SELF,&usage));
	printf("usage.ru_maxrss=%lu\n",usage.ru_maxrss);
	printf("usage.ru_minflt=%lu\n",usage.ru_minflt);
	// the following values do not change and so I don't print them...
	/*
	void* p=sbrk(0);
	printf("p is %p\n",p);
	printf("\tprogram text (etext) %10p\n", &etext);
	printf("\tinitialized data (edata) %10p\n", &edata);
	printf("\tuninitialized data (end) %10p\n", &end);
	*/
}

int main(int argc,char** argv,char** envp) {
	// size of each chunk (10M in this example)
	const size_t chunk_size=10*1024*1024;
	const unsigned int num_chunks=50;
	for(unsigned int i=0;i<num_chunks;i++) {
		printf("trying to allocate block number %d\n",i);
		char* buf=(char*)malloc(chunk_size);
		//bzero(buf,chunk_size);
		if(buf==NULL) {
			perror("error in malloc");
			exit(1);
		}
		show_vmem();
	}
	return EXIT_SUCCESS;
}
