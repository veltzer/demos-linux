/**
 * This is a minimal openmp program.
 * Notice that openmp automatically adjusts to the number of cores that you have.
 * You can ofcourse, change that (reduce or increase number of threads)
 *
 * 		Mark Veltzer
 *
 * EXTRA_COMPILE_FLAGS=-fopenmp
 */

#include <omp.h>
#include <stdio.h>

int main(int argc,char** argv,char** envp) {
	#pragma omp parallel
	printf("Hello from thread %d, nthreads %d\n", omp_get_thread_num(), omp_get_num_threads());
	return 0;
}
