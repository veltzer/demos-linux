#include<stdio.h>
#include<stdlib.h>
#include<strings.h>
#include<unistd.h>

/*
 * This example shows how to use the cleanup framework provided by the standard C library
 * notice that cleanup functions are called in reverse order of their registration
 */

void endfunc1() {
	fprintf(stderr,"Hey! I am doing some cleanup work 1\n");
}
void endfunc2() {
	fprintf(stderr,"Hey! I am doing some cleanup work 2\n");
}

int main(int argc,char** argv,char** envp) {
	atexit(endfunc1);
	atexit(endfunc2);
	// this will end the software immediately
	// _exit(7);
	exit(1);
	return(0);
}
