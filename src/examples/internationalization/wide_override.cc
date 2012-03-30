#include<stdio.h> // for printf(3)
#include<stdlib.h> // for EXIT_SUCCESS

/*
 * This is trying to override the default gcc behaviour
 * for wide strings (overriding the definition of wchar_t).
 *
 *		Mark Veltzer
 */

#define wchar_t char

int main(int argc, char **argv, char **envp) {
	printf("size of char is %d\n", sizeof(char));
	printf("size of wchar_t is %d\n", sizeof(wchar_t));
	return EXIT_SUCCESS;
}
