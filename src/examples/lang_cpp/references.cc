#include<stdio.h> // for printf(3)
#include<stdlib.h> // for EXIT_SUCCESS

/*
 * This example explores the use of references.
 *
 * Things to notice:
 * - the reference is not even stored in it's own place on the stack, or anywhere.
 *	It is a compile time figment of the imagination.
 * - when changing the reference you change the actual value, much like you would do
 *	with a pointer. The syntax is different though. Instead of doing *p=7 you simply
 *	do p=7.
 * - Changing the reference DOES NOT change what value of the reference (it s a pointer
 *	after all) but rather what the reference is pointing to.
 *
 * 		Mark Veltzer
 */

int main(int argc, char **argv, char **envp) {
	int i=7;
	int u=20;
	int& j=i;
	int* k=&i;
	printf("i is %d\n",i);
	printf("j is %d\n",j);
	printf("k is %p\n",k);
	printf("&j is %p\n",&j);
	printf("&j is %p\n",&i);
	printf("&k is %p\n",&k);
	printf("*k is %d\n",*k);
	j=8;
	printf("i is %d\n",i);
	printf("j is %d\n",j);
	printf("k is %p\n",k);
	printf("&j is %p\n",&j);
	printf("&j is %p\n",&i);
	printf("&k is %p\n",&k);
	printf("*k is %d\n",*k);
	*k=9;
	printf("i is %d\n",i);
	printf("j is %d\n",j);
	printf("k is %p\n",k);
	printf("&j is %p\n",&j);
	printf("&j is %p\n",&i);
	printf("&k is %p\n",&k);
	printf("*k is %d\n",*k);
	j=u;
	printf("i is %d\n",i);
	printf("j is %d\n",j);
	printf("k is %p\n",k);
	printf("&j is %p\n",&j);
	printf("&j is %p\n",&i);
	printf("&k is %p\n",&k);
	printf("*k is %d\n",*k);
	return EXIT_SUCCESS;
}
