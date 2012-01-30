#include <iostream>

/*
 * This example investigates the sizeof() of C++ object.
 *
 *	Mark Veltzer
 */

class A {
	public:
		int a;
		char b;
		int c;
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

int main(int argc, char **argv, char **envp) {
	std::cout << "sizeof(A) is " << sizeof(A) << std::endl;
	std::cout << "sizeof(B) is " << sizeof(B) << std::endl;
	//std::cout << "__builtin_offsetof(e,B) " << __builtin_offsetof(e,B) << std::endl;
	std::cout << "CppOffsetOf(B,e) " << CppOffsetOf(B,e) << std::endl;
	A* a=new A();
	a->dosomething();
	A* ab=new B();
	ab->dosomething();
	B* b=new B();
	b->dosomething();
	A anotherA;
	anotherA.dosomething();
	return(0);
}
