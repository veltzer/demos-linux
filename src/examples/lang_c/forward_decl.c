#include <stdio.h> // for printf(3)

/*
 * An example of forward declaration of a function
 */

double add(double a,double b);

int main() {
	printf("5.2+5.6 is %f\n",add(5.2,5.6));
	return 0;
}

double add(double a,double b) {
	return a+b;
}

