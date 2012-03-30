#include <stdio.h> // for printf(3)
#include <stdlib.h> // for EXIT_SUCCESS

/*
 *
 * 			Mark Veltzer
 */

void do_something_with_an_integer(int val) {
	printf("val is %d\n",val);
}

int* do_something_with_intp(int* val) {
	return val+7-7;
}
int t=1945;
const volatile int a=6;

int main(int argc,char** argv,char** envp) {
	int* p=&t;
	p++;
	*p=7;
	printf("a is %d\n",a);
	printf("&a is %p\n",&a);
	printf("&t is %p\n",&t);
	printf("a is %d\n",a);
	/*
	// the next line will produce a compile time error...
	//a=5;
	int* p=(int*)&a;
	*p=7;
	__sync_synchronize();
	printf("*p is %d\n",*p);
	printf("*&a is %d\n",*&a);
	int* foo=(int*)&a;
	foo=do_something_with_intp(foo);
	printf("*foo is %d\n",*foo);
	do_something_with_an_integer(a);
	*/
	return EXIT_SUCCESS;
}
