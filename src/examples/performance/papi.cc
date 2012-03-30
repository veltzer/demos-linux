#include<stdio.h> // for printf(3)
#include<unistd.h> // for sleep(3)
#include<papi.h> // for PAPI_* functions
#include<stdlib.h> // for EXIT_SUCCESS

//#include"us_helper.hh"

/*
 * Demo for using the PAPI performance counters library.
 * 
 * References:
 * http://icl.cs.utk.edu/papi/index.html
 *
 *	Mark Veltzer
 *
 * EXTRA_LIBS=-lpapi
 */

void print_counters(long long* arr,int num) {
	for(int i=0;i<num;i++) {
		printf("%d: %llu\n",i,arr[i]);
	}
}

int main(int argc, char **argv, char **envp) {
	printf("starting up...\n");
	int counters=PAPI_num_counters();
	printf("PAPI_num_counters() returned %d\n",counters);
	int components=PAPI_num_components();
	printf("PAPI_num_components() returned %d\n",components);

	// array for counters
	long long* arr=new long long[counters];
	float rtime,ptime,ipc,mflops;
	long long ins,flpops;

	int ret=PAPI_ipc(&rtime,&ptime,&ins,&ipc);
	printf("PAPI_ipc() returned:\n");
	printf("\trtime=%f\n",rtime);
	printf("\tptime=%f\n",ptime);
	printf("\tins=%lld\n",ins);
	printf("\tipc=%f\n",ipc);
	printf("\tret=%d\n",ret);

	ret=PAPI_flops(&rtime,&ptime,&flpops,&mflops);
	printf("PAPI_flops() returned:\n");
	printf("\trtime=%f\n",rtime);
	printf("\tptime=%f\n",ptime);
	printf("\tflpops=%lld\n",flpops);
	printf("\tmflops=%f\n",mflops);
	printf("\tret=%d\n",ret);


	ret=PAPI_read_counters(arr,counters);
	printf("ret is %d\n",ret);
	print_counters(arr,counters);
	sleep(1);
	ret=PAPI_read_counters(arr,counters);
	printf("ret is %d\n",ret);
	print_counters(arr,counters);
	return EXIT_SUCCESS;
}
