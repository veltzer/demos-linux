#include<stdio.h> // for printf(3)
#include<stdlib.h> // for EXIT_SUCCESS

/*
	this is an example of a loop that gcc cannot optimize away...
	Consider the question why ? Doesn't gcc simulate your code
	in order to optimize it away ? The answer is that it does
	not simulate your code but rather understand the underlying
	construct to deduce what you are doing and use a predefined
	set of formulas to precalculate the result...

	You can see that indeed it is not optimized when you disassemble
	the code.

	Compare the second loop to the first.

		Mark Veltzer
*/

int main(int argc,char** argv,char** envp) {
	int sum=0;
	for(int i=0;i<100;i++) {
		sum+=i;
	}
	printf("sum is %d\n",sum);
	for(int i=0;i<100;i++) {
		sum+=i*i;
	}
	printf("sum is %d\n",sum);
	return EXIT_SUCCESS;
}
