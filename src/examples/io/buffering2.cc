#include<stdio.h> // for printf(3)

/*
 * Example of buffering of stdout causing loss of messages...
 *
 * 		Mark Veltzer
 */

int main(int argc,char** argv,char** envp) {
	printf("stdout data");
	fprintf(stderr,"stderr data");
	printf("stdout data2");
	fprintf(stderr,"stderr data2");
	printf("\n");
	printf("help me user!!!!");
	*(char*)0=0;
}
