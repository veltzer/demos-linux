#include <sys/types.h> // for ftok(3)
#include <sys/ipc.h> // for ftok(3)
#include <stdio.h> // for printf(3)
#include <stdlib.h> // for EXIT_SUCCESS

/*
 * This example shows that you actually need to point to a real existing file
 * name in order for ftok to succeed...
 *
 * 		Mark Veltzer
 */

int main(int argc,char** argv,char** envp) {
	key_t k1=ftok("/tmp/no_such_file", 17);
	printf("key with no file is %d\n",k1);
	key_t k2=ftok("/etc/passwd", 17);
	printf("key with file is %d\n",k2);
	return EXIT_SUCCESS;
}
