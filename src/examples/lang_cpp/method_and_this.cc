#include<iostream> // for std::cout, std::endl
#include<stdlib.h> // for EXIT_SUCCESS

/*
 * An example of passing object and method and applying the method
 * on the object...
 * This is sometimes called a functor (object+method). You can find
 * similar approaches in boost's bind function.
 *
 *		Mark Veltzer
 */

class A {
	public:
		int val;
		void run() {
			std::cout << val << std::endl;
		}
};

void apply(A& a,void (A::*method)()) {
	(a.*method)();
}

int main(int argc, char **argv, char **envp) {
	A a;
	a.val=7;
	apply(a,&A::run);
	return EXIT_SUCCESS;
}
