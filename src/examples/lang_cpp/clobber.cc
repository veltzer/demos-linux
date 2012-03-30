#include<stdio.h> // for printf(3)
#include<stdlib.h> // for EXIT_SUCCESS

/*
 * This is a demo that shows C++ allows to clobber variable names in serveral ways:
 * - inheritance.
 * - local parameters.
 *
 *		Mark Veltzer
 */

class A {
	protected:
		int a;
		int b;
};

class B: public A {
	public:
		int c;
		int b;
	void doSomething(int a) {
		a=5;
		b=6;
		c=8;
		printf("a is %d\n",a);
		printf("b is %d\n",b);
		printf("c is %d\n",c);
		printf("this->a is %d\n",this->a);
		printf("this->b is %d\n",this->b);
		printf("this->c is %d\n",this->c);
	}
};

int main(int argc,char** argv,char** envp) {
	B b;
	b.doSomething(7);
	return EXIT_SUCCESS;
}
