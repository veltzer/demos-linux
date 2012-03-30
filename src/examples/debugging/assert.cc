#include<stdio.h> // for fprintf(3)
#include<stdlib.h> // for EXIT_SUCCESS

#define __stringify_1(x) # x
#define __stringify(x) __stringify_1(x)

#define ASSERT(expr)\
	if(!(expr)) {\
		fprintf(stderr,"ERROR! " __stringify_1(expr) "\n");\
	}


int main(int argc,char** argv,char** envp) {
	int x=1;
	ASSERT(x==2);
	return EXIT_SUCCESS;
}
