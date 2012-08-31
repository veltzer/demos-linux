/*
	This file is part of the linuxapi project.
	Copyright (C) 2011, 2012 Mark Veltzer <mark.veltzer@gmail.com>

	The linuxapi package is free software; you can redistribute it and/or
	modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 2.1 of the License, or (at your option) any later version.

	The linuxapi package is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public
	License along with the GNU C Library; if not, write to the Free
	Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
	02111-1307 USA.
*/

#include<firstinclude.h>
#include<stdio.h> // for printf(3)
#include<omp.h> // for omp_get_wtime(3), omp_get_thread_num(3), omp_get_num_procs(3), omp_get_max_threads(3)
#include<stdlib.h> // for EXIT_SUCCESS

/*
* This is an example of a parallel block of code. The for loop is not parallel
* here, it is the block containting the for loop that is parallel.
*
* EXTRA_COMPILE_FLAGS=-fopenmp
*/

int main(int argc,char** argv,char** envp) {
	printf("C/OpenMP demo\n");
	printf("Number of processors available=%d\n",omp_get_num_procs());
	printf("Number of threads=%d\n",omp_get_max_threads());

	/*
	* Measure the time before we start
	*/
	double wtime=omp_get_wtime();

	omp_set_num_threads(3);
	int id;
	#pragma omp parallel default(shared) private(id)
	{
		id=omp_get_thread_num();
		for(double i=1; i<10; i++) {
			printf(" i=%f, i*i=%f, %d\n", i, i*i,id);
		};
		printf("This is process %d\n",id);
	}
	/*
	* Finish up by measuring the elapsed time.
	*/
	wtime=omp_get_wtime()-wtime;
	printf("\n");
	printf("Normal end of execution.\n");
	printf("Elapsed wall clock time=%f\n",wtime);
	return EXIT_SUCCESS;
}
