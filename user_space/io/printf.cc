#include <stdio.h> // for printf(3)

/*
 *      This demo shows how to print an unsigned long in a big hexa format...
 *
 *              Mark Veltzer
 *
 * EXTRA_LIBS=
 */
int main(int argc, char **argv, char **envp) {
	unsigned long i = 0xabcd0123;
	void          *p = (void *)i;

	printf("void* printed as '%%p' is %p\n", p);
	printf("'%%lx' is %lx\n", i);
	printf("'%%lX' is %lX\n", i);
	printf("'%%#lx' is %#lx\n", i);
	printf("'%%#lX' is %#lX\n", i);
	printf("with conversion '%%lX' is %lX\n", (unsigned long)p);
	return(0);
}
