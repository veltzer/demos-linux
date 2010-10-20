#include<stdio.h>
#include<stdlib.h>

int main(int argc,char** argv,char** envp) {
	const int size=100;
	char* buffer;
	int i;
	fprintf(stderr,"starting\n");
	buffer=(char*)malloc(size);
	for(i=0;i<200;i++) {
		fprintf(stderr,"i is %d\n",i);
		buffer[i]=1;
	}
	fprintf(stderr,"ending\n");
	return(0);
}
