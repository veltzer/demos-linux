#include<papi.h> // for PAPI_* functions
#include<unistd.h> // for sleep(3)
#include<stdio.h> // for printf(3), perror(3)
#include<stdlib.h> // for exit(3), atoi(3)
#include<errno.h> // for perror(3)

#include"us_helper.hh"

/*
 * Demo for using the PAPI library for RDTSC.
 * This example also proves that RDTSC is exactly what the PAPI library
 * is using under the hood.
 * 
 * References:
 * http://icl.cs.utk.edu/papi/index.html
 *
 *              Mark Veltzer
 *
 * EXTRA_LIBS=-lpapi
 */

void handle_error(int val) {
	perror("had an error");
	exit(1);
}

long long dummy_work(int limit) {
	int count=0;
	for(int j=0;j<limit;j++) {
		for(int i=0;i<limit;i++) {
			count+=dummy_work(limit-1);
			//count+=i;
		}
	}
	return count;
}

int main(int argc,char** argv,char** envp) {
	int retval;
	retval = PAPI_library_init(PAPI_VER_CURRENT);
	if (retval != PAPI_VER_CURRENT && retval > 0) {
		fprintf(stderr,"PAPI library version mismatch!\n");
		exit(1);
	}
	if (retval < 0)
		handle_error(retval);

	// lets start measuring...
	long long c1=PAPI_get_real_cyc();
	ticks_t t1=getticks();
	//dummy_work(atoi(argv[1]));
	//sleep(1);
	//sleep(1);
	long long c2=PAPI_get_real_cyc();
	ticks_t t2=getticks();
	printf("Wallclock cycles: %lld\n",c2-c1);
	printf("Wallclock cycles: %lld\n",t2-t1);
	printf("c1 is %lld\n",c1);
	printf("c2 is %lld\n",c2);
	printf("t1 is %lld\n",t1);
	printf("t2 is %lld\n",t2);
	return 0;
}
