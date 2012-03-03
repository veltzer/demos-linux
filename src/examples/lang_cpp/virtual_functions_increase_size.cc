#include <iostream> // for std::cout, std::endl
#include <assert.h> // for assert(3)

/*
 * This example shows that a single virtual function in an object increases it's size.
 *
 *	Mark Veltzer
 */

// A1 and A2 are exactly the same except the virtual in A2

class A1 {
	public:
		int a;

		A1(int val) {
			a=val;
		}
		void dosomething() {
			for(int i=0;i<4;i++) {
				std::cout << "Hello from A, i is " << i << std::endl;
			}
		}
};

class A2 {
	public:
		int a;

		A2(int val) {
			a=val;
		}

		virtual void dosomething() {
			for(int i=0;i<4;i++) {
				std::cout << "Hello from A, i is " << i << std::endl;
			}
		}
};

int main(int argc, char **argv, char **envp) {
	std::cout << "sizeof(A1) is " << sizeof(A1) << std::endl;
	std::cout << "sizeof(A2) is " << sizeof(A2) << std::endl;
	A2 obj1(4),obj2(5);
	int* p1=(int*)&obj1;
	int* p2=(int*)&obj2;
	assert(*p1==*p2);
	assert(*(p1+1)=4);
	assert(*(p2+1)=5);
	return(0);
}
