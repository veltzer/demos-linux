#include<stdio.h> // for printf(3)
#include<stdlib.h> // for EXIT_SUCCESS

void printHello() {
	static int count=0;
	count++;
	printf("function was called %d times\n",count);
	printf("Hello, World!\n");
}

int main(int argc,char** argv,char** envp) {
	printHello();
	printHello();
	printHello();
	printHello();
	return EXIT_SUCCESS;
}
