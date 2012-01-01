/**
 * This is a minimal openmp program.
 *
 * 		Mark Veltzer
 *
 * EXTRA_COMPILE_FLAGS=-fopenmp
 */

#include<stdlib.h>
#include<stdio.h>
#include<omp.h>

int main(int argc,char** argv,char** envp) {
	int id;
	double wtime;
	printf ( "\n" );
	printf ( "HELLO\n" );
	printf ( " C/OpenMP version\n" );
	printf ( "\n" );
	printf ( " Number of processors available = %d\n", omp_get_num_procs ( ) );
	printf ( " Number of threads = %d\n", omp_get_max_threads ( ) );
	wtime = omp_get_wtime ( );

#pragma omp num_threads(2);
#pragma omp parallel default(shared) private(id)
	{
		id = omp_get_thread_num ( );
		for (double i=1; i<1e5; i++) {printf (" i = %f, i*i = %f\n", i, i*i);};
		printf (" This is process %d\n", id );
	}
	/*
	 * Finish up by measuring the elapsed time.
	 * */
	wtime = omp_get_wtime ( ) - wtime;
	printf ( "\n" );
	printf ( "HELLO\n" );
	printf ( " Normal end of execution.\n" );
	printf ( "\n" );
	printf ( " Elapsed wall clock time = %f\n", wtime );
	return 0;
}
