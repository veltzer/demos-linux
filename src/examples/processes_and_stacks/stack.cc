#include<string.h> // for memset(3)
#include<stdio.h> // for printf(3)
#include<sys/resource.h> // for getrlimit(2)

#include"us_helper.hh" // our own helper

/*
 * This example shows the stack limitations of a regular program.
 *
 * Notes:
 * - this is why it's a bad idea to store stuff on the stack.
 * - note that without the memset there is no problem since the
 *	memory is not really allocated.
 *
 * 			Mark Veltzer
 */

// print the current limit for stack usage for the process
void print_limit(void) {
	struct rlimit rlim;
	CHECK_NOT_M1(getrlimit(RLIMIT_STACK,&rlim));
	printf("rlim_cur is %ld\n",rlim.rlim_cur);
	printf("rlim_max is %ld\n",rlim.rlim_max);
}

// a single function using lots of stack. Either fake (without
// memset) or real (with memset)
void func(int m,bool mems) {
	const unsigned int size=m*1024*1024;
	char data[size];
	if(mems) {
		memset(data,0,size);
	}
}

// a loop trying to use more and more stack
void loop(bool mems) {
	printf("mems is %d\n",mems);
	for(unsigned int m=1;m<20;m++) {
		printf("m is %d\n",m);
		func(m,mems);
	}
}

int main(int argc,char** argv,char** envp) {
	print_limit();
	loop(false);
	loop(true);
	return EXIT_SUCCESS;
}
