#include<stdio.h>

/*
 * This examples shows the effect of the nice scheduling system.
 *
 * It should fork two versions of heavy work each with a different nice level.
 *
 * References:
 * man 2 nice
 *
 * 	Mark Veltzer
 */

int main(int argc,char** argv,char** envp) {
	float sum;
	for(unsigned int i=0;i<1000000;i++) {
		for(unsigned int j=0;j<100000000;j++) {
			sum+=j;
		}
		printf("i is %d\n",i);
	}
	return 0;
}
