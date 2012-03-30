#include <iostream> // for std::cout, std::endl
#include <stdlib.h> // for EXIT_SUCCESS

/*
 * This example investigates the sizeof() of C++ object.
 *
 *	Mark Veltzer
 */

class A {
	public:
		int a;
		int c;
		char b;
		char d;

		void dosomething() {
			for(int i=0;i<4;i++) {
				std::cout << "Hello from A, i is " << i << std::endl;
			}
		}
};

class B: public A {
	public:
		int e;
		char f;
		void dosomething() {
			for(int i=0;i<4;i++) {
				std::cout << "Hello from B, i is " << i << std::endl;
			}
		}
};
#define CppOffsetOf(className, FieldName) ((char *)(&(((className *)1)->FieldName)) - (char *)1)

class C {
};

int main(int argc, char **argv, char **envp) {
	std::cout << "sizeof(A) is " << sizeof(A) << std::endl;
	std::cout << "sizeof(B) is " << sizeof(B) << std::endl;
	std::cout << "sizeof(C) is " << sizeof(C) << std::endl;
	//std::cout << "__builtin_offsetof(e,B) " << __builtin_offsetof(e,B) << std::endl;
	std::cout << "CppOffsetOf(B,e) " << CppOffsetOf(B,e) << std::endl;
	std::cout << "CppOffsetOf(A,a) " << CppOffsetOf(A,a) << std::endl;
	std::cout << "CppOffsetOf(A,b) " << CppOffsetOf(A,b) << std::endl;
	std::cout << "CppOffsetOf(A,c) " << CppOffsetOf(A,c) << std::endl;
	std::cout << "CppOffsetOf(A,d) " << CppOffsetOf(A,d) << std::endl;
	A* a=new A();
	a->dosomething();
	A* ab=new B();
	ab->dosomething();
	B* b=new B();
	b->dosomething();
	A anotherA;
	anotherA.dosomething();
	return EXIT_SUCCESS;
}
