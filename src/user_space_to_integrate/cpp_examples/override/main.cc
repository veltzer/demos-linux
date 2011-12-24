#include<stdio.h>
#include<stdlib.h>

int main(int argc,char** argv,char** envp) {
	printf("HOME is %s\n",getenv("HOME"));
	return 0;
}
