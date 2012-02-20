/*
 * This is an example of a parallel block of code. The for loop is not parallel
 * here, it is the block containting the for loop that is parallel.
 *
 * 		Mark Veltzer
 *
 * EXTRA_COMPILE_FLAGS=-fopenmp
 */

#include<stdio.h> // for printf(3)
#include<omp.h> // for omp_*(3)

int main(int argc,char** argv,char** envp) {
	int id;
	double wtime;
	printf("C/OpenMP demo\n");
	printf("Number of processors available=%d\n",omp_get_num_procs());
	printf("Number of threads=%d\n",omp_get_max_threads());

	/*
	 * Measure the time before we start
	 */
	wtime=omp_get_wtime();

	omp_set_num_threads(3);
	#pragma omp parallel default(shared) private(id)
	{
		id=omp_get_thread_num();
		for(double i=1; i<10; i++) {
			printf(" i = %f, i*i = %f, %d\n", i, i*i,id);
		};
		printf("This is process %d\n",id);
	}
	/*
	 * Finish up by measuring the elapsed time.
	 */
	wtime=omp_get_wtime()-wtime;
	printf("\n");
	printf("Normal end of execution.\n" );
	printf("Elapsed wall clock time=%f\n",wtime);
	return 0;
}
