#include <stdio.h>
#include <cpp/test/helloworld.h>

/*
 * This is a small dll implementing a function which outputs "Hello, World!\n".
 */
void helloworld(void) {
	printf("Hello, World!\n");
}
