#include<stdio.h> // for printf(3)
#include<values.h> // for MAXINT, MININT, MAXFLOAT, MINFLOAT

// This example prints the limits of various types on the system using
// the values.h file. limits.h may also be used for this.
//
// 	Mark Veltzer

int main(int argc,char** argv,char** envp) {
	printf("MAXINT is %d\n",MAXINT);
	printf("MININT is %d\n",MININT);
	printf("MAXFLOAT is %f\n",MAXFLOAT);
	printf("MINFLOAT is %f\n",MINFLOAT);
	printf("MAXDOUBLE is %lf\n",MAXDOUBLE);
	printf("MINDOUBLE is %lf\n",MINDOUBLE);
	return 0;
}
