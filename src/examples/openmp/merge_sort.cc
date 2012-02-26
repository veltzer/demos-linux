/*
 * An example of writing a merge sort algorithm using open mp.
 *
 * 			Mark Veltzer
 *
 * EXTRA_COMPILE_FLAGS=-fopenmp
 */

#include <cmath>
#include <stdio.h> // for printf(3)
#include <omp.h>

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
	return 0;
}
