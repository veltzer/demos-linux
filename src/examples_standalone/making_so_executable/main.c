#include<stdio.h>
#include<stdlib.h>
#include"add.h"

int main(int argc, char **argv, char **envp) {
	printf("2+2 is %d\n", func(2, 2));
	return(0);
}
