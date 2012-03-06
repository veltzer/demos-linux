#include<stdio.h>

/*
 * Example showing forgetting for initialization of members
 */

class A {
	public:
		int a;
		int b;
		A() {
			a=6;
		}
};

int main(int argc,char** argv,char** envp) {
	A a;
	printf("a.a is %d\n",a.a);
	printf("a.b is %d\n",a.b);
	return 0;
}
