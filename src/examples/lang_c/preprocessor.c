#include<stdio.h> // for printf(3)

/*
 * An example showing how to use the preprocessor in order to avoid
 * repetative code.
 *
 * 			Mark Veltzer
 */

#define make_add(t) t add ## t(t a,t b) { return a+b; }

make_add(float);
make_add(double);

/*
float addfloat(float a,float b) {
	return a+b;
}
double adddouble(double a,double b) {
	return a+b;
}
*/

int main() {
	float x=4.5f;
	float y=3.2f;
	printf("%f+%f=%f\n",x,y,addfloat(x,y));
	return 0;
}
