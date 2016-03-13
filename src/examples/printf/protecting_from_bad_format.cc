#include<stdlib.h>	// for EXIT_SUCCESS
#include<unistd.h>	// for malloc(3)
#include<stdio.h>	// for printf(3)

/*
 * This example proves that glibc does not protect against
 * accidentaly passing a non const char* string as format
 * argument to printf(3)
 */

int main(int argc, char** argv, char** envp) {
	char* malicious_string=(char*)malloc(20);
	printf(malicious_string, 45);
	return EXIT_SUCCESS;
}
