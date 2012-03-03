#include <iostream> // for std::cout, std::endl
#include <typeinfo> // for typeid

/*
 * Demo the typeid function of C++, with RTTI
 *
 *		Mark Veltzer
 */

class A {
};
class B {
	public:
		virtual void doit() {
			std::cout << "Hello, World!" << std::endl;
		}
};

int main(int argc, char **argv, char **envp) {
	A a;
	B b;
	std::cout << typeid(a).name() << std::endl;
	std::cout << typeid(b).name() << std::endl;
	std::cout << typeid(std::cout).name() << std::endl;
	return(0);
}
