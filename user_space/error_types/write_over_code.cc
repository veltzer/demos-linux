#include <stdio.h>

/*
 * This demo shows how to create a segmentation fault by trying to write
 * over a function. This is impossible because of the MMU and the fact
 * that text pages are write protected (they are r-x).
 *
 * 			Mark Veltzer
 */

void my_func() {
	int x=7;
	printf("x is %d\n",x);
}

int main(int argc,char** argv,char** envp) {
	int* t=(int*)&my_func;
	*t=8;
	return(0);
}
