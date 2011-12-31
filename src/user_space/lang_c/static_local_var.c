#include <stdio.h> // for printf(3)

void printHello() {
	static int count=0;
	count++;
	printf("function was called %d times\n",count);
	printf("Hello, World!\n");
}

int main() {
	printHello();
	printHello();
	printHello();
	printHello();
	return 0;
}
