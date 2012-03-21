#include<stdio.h>

int doit(int a,int b) __attribute__((noinline));
int doit(int a,int b) {
	return a+b;
}

int main(int argc,char** argv,char** envp) {
	printf("did you know that 2+2=%d\n",doit(2,2));
	return 0;
}
