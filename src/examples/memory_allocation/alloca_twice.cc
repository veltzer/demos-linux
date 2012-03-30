#include<strings.h> // for bzero(3)
#include<alloca.h> // for alloca(3)
#include<stdio.h> // for printf(3)

#include"us_helper.hh"

/*
 * This example investigates what happens if you call alloca(3) twice...
 *
 * Results:
 * - you can call alloca more than once.
 * - as a result you CANNOT free memory obtained by alloca(3).
 * - alloca rounds stuff up by 8 bytes or some such.
 *
 * 	Mark Veltzer
 */

int main(int argc,char** argv,char** envp) {
	for(int i=0;i<10;i++) {
		char* p=(char*)alloca(10);
		bzero(p,10);
		printf("p1 is %p\n",p);
	}
	return EXIT_SUCCESS;
}
