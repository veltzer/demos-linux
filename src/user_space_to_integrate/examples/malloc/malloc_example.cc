#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>

const int page_size=4096;
const int page_number=2000;

/*
 * This example demostrates that malloc doesnt actually allocate
 * physical memory and that just touching the memory slowly allocates
 * it
 *
 * watch it using:
 * while [[ true ]]; do ps -e -o comm,size,rss | fgrep malloc_example; sleep 1;done
 */

int main(int argc,char** argv,char** envp) {
	char* p=(char*)malloc(page_size*page_number);
	for(int i=0;i<page_number;i++) {
		p[i*page_size]=0;
		fprintf(stderr,"Just touched page number %d\n",i);
		sleep(1);
	}
	return 0;
}
