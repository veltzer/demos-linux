#include<stdio.h> // for printf(3)
#include<math.h> // for sin(3)
#include<stdlib.h> // for EXIT_SUCCESS

int main(int argc,char** argv,char** envp) {
	double x=4.56;
	printf("sin(%lf)=%lf\n",x,sin(x));
	return EXIT_SUCCESS;
}
