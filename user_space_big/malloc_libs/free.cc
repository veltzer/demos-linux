#include<stdio.h>
#include<stdlib.h>
#ifdef DMALLOC
#include <dmalloc.h>
#endif

int main(int argc,char** argv,char** envp) {
	fprintf(stderr,"starting\n");
	char* buffer=(char*)malloc(100);
	fprintf(stderr,"ending\n");
	return(0);
}
