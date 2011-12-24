#include<stdio.h>

/*
	This example shows that the virtula table for a C++ object
	is dumped in *the file that implement the first real (non
	pure virtual) function in the declaration of the class*.
*/

class A {
	public:
		virtual void foo(void);
		virtual void bar(void);
};

void A::foo(void) {
	printf("this is foo");
}
/*
void A::bar(void) {
	printf("this is foo");
}
*/
