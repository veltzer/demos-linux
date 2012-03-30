#include<stdio.h> // for fprintf(3)
#include<stdlib.h> // for EXIT_SUCCESS

/*
 * This is a demo of a segmentation fault created by calling a function badly.
 *
 * 		Mark Veltzer
 */

void func() {
	int i;
	const char* y __attribute__((unused))="123123dsfshfdkj$%^$%&^^%&@";
	for(i=0;i<100;i++) {
		fprintf(stderr,"i is %d\n",i);
	}
}

int main(int argc,char** argv,char** envp) {
	char* f=(char*)func;
	f++;
	f++;
	void (*g)()=(void (*)())f;
	g();
	return EXIT_SUCCESS;
}
