#include <stdio.h> // for printf(3)

#include "us_helper.hh"

/*
 * This is an example of an object deallocating itself.
 * The idea is to put a "delete this" at the VERY END of some
 * method.
 * You cannot put this in the destructor since you will cause
 * an infinite regress.
 *
 * What are the problems with this approach ?!?
 * Well - "delete this" requires that the object in question was
 * allocated on the heap and not say, stack or data segment. In
 * that cause doing "delete this" may cause a segfault or maybe
 * even a silent bug which is worse at times.
 *
 *		Mark Veltzer
 */

class A {
	private:
		int secret;
	public:
		A(void) {
			printf("This is the constructor\n");
		}
		~A(void) {
			printf("This is the destructor\n");
		}
		void doit(void) {
			// the next line must be the very LAST line
			// in this methods code (after any business
			// logic).
			delete this;
		}
};

// here is an object that causes infinite recursion in it's destructor...
class B {
	public:
		~B(void) {
			delete this;
		}
};

int main(int argc, char **argv, char **envp) {
	A* a=new A();
	a->doit();
	// this is a problem since the next allocation is on the stack and not
	// on the heap...
	//A aa;
	//aa.doit();
	// the next code will cause infinite recursion and error...
	//B* b=new B();
	//delete b;
	return(0);
}
