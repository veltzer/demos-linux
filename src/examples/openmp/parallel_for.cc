#include<cmath> // for std::sin
#include<stdio.h> // for printf(3)
#include<omp.h> // for #pragma omp parallel for
#include<stdlib.h> // for EXIT_SUCCESS

/*
 * A demo calculating the sin table with openmp.
 *
 * 			Mark Veltzer
 *
 * EXTRA_COMPILE_FLAGS=-fopenmp
 */

int main(int argc,char** argv,char** envp) {
	const int size = 256;
	double sinTable[size];
	#pragma omp parallel for
	for(int n=0; n<size; ++n) {
		printf("thread %d calculating %d\n", omp_get_thread_num(), n);
		sinTable[n] = std::sin(2 * M_PI * n / size);
	}
	// the table is now initialized
	for(int i=0;i<size;i++) {
		printf("sinTable[%d]=%lf\n",i,sinTable[i]);
	}
	return EXIT_SUCCESS;
}
