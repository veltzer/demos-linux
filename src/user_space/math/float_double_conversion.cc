#include <stdio.h>
#include <string.h>
#include <math.h> // for powf(3), pow(3)
#include <stdlib.h> // for atof(3)

/*
 * This is an exploration of how to compiler coverts between float
 * double.
 *
 * Note:
 * Some of these lines will be caught if you add -Wconversion flag to gcc.
 *
 *		Mark Veltzer
 *
 * EXTRA_LIBS=
 */

/*
 * A regular float function...
 */
float my_float_func(float a, float b) {
	// the next line will keep the compiler from optimizing away my function
	// (damn smartass compiler...)
	asm ("");
	return a*b;
}
float my_float_func(float a, float b) __attribute__((noinline));
/*
 * A double precision function...
 */
double my_double_func(double a, double b) {
	// the next line will keep the compiler from optimizing away my function
	// (damn smartass compiler...)
	asm ("");
	return a*b;
}
double my_double_func(double a, double b) __attribute__((noinline));

int main(int argc, char **argv, char **envp) {
	if(argc!=3) {
		fprintf(stderr,"usage %s [float] [double]\n",argv[0]);
		return -1;
	}
	// a float and double which will NEVER be optimized away by the compiler
	// since their origin is the user...
	float f=(float)atof(argv[1]);
	double d=atof(argv[2]);
	printf("%f\n",my_float_func(f,f));
	printf("%lf\n",my_double_func(f,f));
	printf("%f\n",my_float_func(d,d));
	printf("%lf\n",my_double_func(d,d));
	printf("%f\n",my_float_func(5.5f,6.7f));
	printf("%lf\n",my_double_func(5.5f,6.7f));
	printf("%f\n",my_float_func(5.5,6.7));
	printf("%lf\n",my_double_func(5.5,6.7));
	return(0);
}
