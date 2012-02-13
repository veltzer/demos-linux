#include <stdio.h> // for printf(3)

/*
 *      This demo shows how to print an unsigned long in a big hexa format...
 *
 *              Mark Veltzer
 */
int main(int argc, char **argv, char **envp) {
	unsigned long i = 0xabcd0123;
	void          *p = (void *)i;
	char* cp=(char*)i;

	printf("void* printed as '%%p' is %p\n", p);
	printf("char* (and any other pointer) printed as '%%p' is %p\n", cp);
	printf("'%%lx' is %lx\n", i);
	printf("'%%lX' is %lX\n", i);
	printf("'%%#lx' is %#lx\n", i);
	printf("'%%#lX' is %#lX\n", i);
	printf("with conversion '%%lX' is %lX\n", (unsigned long)p);
	int width=5;
	int num=4;
	printf("printing with non compile time width %*d\n", width, num);
	return(0);
}
