#include <stdio.h> // for printf(3)

#include "us_helper.hh"

/*
 * This example shows that private in C++ is no protection at all...
 *
 *                              Mark Veltzer
 * EXTRA_LIBS=
 */

/*
 * This is supposed to be a class which does not allow access to it's secret private,
 * only through accessors...
 */
class A {
	private:
		int secret;
	public:
		A(void) {
			secret=0;
		}
		int getSecret(void) {
			return secret;
		}
		void setSecret(int val) {
			secret=val;
		}
};
/*
 * A similar class but with virtual methods...
 */
class B {
	private:
		int secret;
	public:
		B(void) {
			secret=0;
		}
		int getSecret(void) {
			return secret;
		}
		void setSecret(int val) {
			secret=val;
		}
		virtual void doit(void) {
			printf("in doit\n");
		}
};

void setSecret(void* a, int val, bool has_virt) {
	int* p=(int*)a;
	if(has_virt) {
		p++;
	}
	*p=val;
}

int main(int argc, char **argv, char **envp) {
	A a;
	B b;
	printf("a.getSecret() is %d\n",a.getSecret());
	printf("b.getSecret() is %d\n",b.getSecret());
	// this next line will not compile... compiler enforces access restriction...
	//a.secret=7;
	setSecret(&a,7,false);
	setSecret(&b,7,true);
	printf("a.getSecret() is %d\n",a.getSecret());
	printf("b.getSecret() is %d\n",b.getSecret());
	return(0);
}
