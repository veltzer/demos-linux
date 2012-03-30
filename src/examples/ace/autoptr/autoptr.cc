#include<ace/Auto_Ptr.h>
#include<iostream>

/*
 * EXTRA_CMDS=pkg-config --cflags --libs ACE
 */

/* This example demonstrates the use of auto_ptr which takes
 * care of doing the "delete" when the auto_ptr object is destroyed
 * (goes out of scope)
 */

class A {
private:
	const char *p;
public:
	A(const char *ip) : p(ip) {
		std::cerr << "in constructor of " << p << std::endl;
	}


	~A() {
		std::cerr << "in destructor of " << p << std::endl;
	}
};

int ACE_TMAIN(int, ACE_TCHAR *[]) {
	auto_ptr<A> a(new A("a"));
	new A("b");

	return(0);
}
