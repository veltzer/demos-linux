#include <omp.h> // for #pragma omg parallel
#include <stdio.h> // for printf(3)
#include <stdlib.h> // for EXIT_SUCCESS

/*
 * This is a minimal openmp program.
 * Notice that openmp automatically adjusts to the number of cores that you have.
 * You can ofcourse, change that (reduce or increase number of threads)
 *
 * 		Mark Veltzer
 *
 * EXTRA_COMPILE_FLAGS=-fopenmp
 */

int main(int argc,char** argv,char** envp) {
	#pragma omp parallel
	printf("Hello from thread %d, nthreads %d\n", omp_get_thread_num(), omp_get_num_threads());
	return EXIT_SUCCESS;
}
