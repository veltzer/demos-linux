/**
 * A demo calculating the sin table with openmp.
 *
 * 			Mark Veltzer
 *
 * EXTRA_COMPILE_FLAGS=-fopenmp
 */

#include <cmath>

int main(int argc,char** argv,char** envp) {
	const int size = 256;
	double sinTable[size];
	#pragma omp parallel for
	for(int n=0; n<size; ++n)
	sinTable[n] = std::sin(2 * M_PI * n / size);
	// the table is now initialized
	return 0;
}
