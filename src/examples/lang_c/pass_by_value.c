#include<stdio.h> // for printf(3)
#include<stdlib.h> // for EXIT_SUCCESS

void addone(double* a) {
	*a+=1.0;
}

int main(int argc, char **argv, char **envp) {
	double g=4.5;
	printf("the address of g is %p\n",&g);
	addone(&g);
	printf("g is %f\n",g);
	return EXIT_SUCCESS;
}
