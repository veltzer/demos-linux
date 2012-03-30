#include<stdio.h> // for printf(3)

void addone(double* a) {
	*a+=1.0;
}

int main() {
	double g=4.5;
	printf("the address of g is %p\n",&g);
	addone(&g);
	printf("g is %f\n",g);
	return 0;
}
