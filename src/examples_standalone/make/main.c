#include<stdio.h> // for printf(3)
#include<stdlib.h> // for EXIT_SUCCESS
#include"add.h"

int main(int argc,char** argv,char** envp) {
	printf("5.2+5.6 is %f\n",add(5.2,5.6));
	return EXIT_SUCCESS;
}
