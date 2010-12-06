#include <string.h> // for memset(3)

/*
 * This example shows the stack limitations of a regular program
 *
 * Notes:
 * - this is why it's a bad idea to store stuff on the stack.
 * - note that without the memset there will be no problem.
 *
 * 			Mark Veltzer
 */

void func(void) {
	const unsigned int m=10;
	const unsigned int size=m*1024*1024;
	char data[size] __attribute((unused));
	memset(data,0,size);
}

int main(int argc,char** argv,char** envp) {
	func();
	return 0;
}
