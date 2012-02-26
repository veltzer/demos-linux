/*
 * An example of writing a merge sort algorithm using open mp.
 *
 * 			Mark Veltzer
 *
 * EXTRA_COMPILE_FLAGS=-fopenmp
 */

#include<stdio.h> // for printf(3)
#include<omp.h> // for openmp pragmas and functions
#include<stdlib.h> // for random(3)
#include<string.h> // for memcpy(3)

void mergesort(int* arr,unsigned int from,unsigned int to,int* scratch) {
	if(from+1==to) {
		return;
	}
	unsigned int length=to-from;
	unsigned int mid=(from+to)/2;
	omp_set_num_threads(2);
	#pragma omp parallel sections
	{
		#pragma omp section
		mergesort(arr,from,mid,scratch);
		#pragma omp section
		mergesort(arr,mid,to,scratch);
	}
	memcpy(scratch+from,arr+from,length*sizeof(int));
	unsigned int i=mid;
	unsigned int j=from;
	unsigned int target=from;
	while(target<to) {
		if(i<to && j<mid) {
			if(scratch[i]<scratch[j]) {
				arr[target]=scratch[i];
				i++;
			} else {
				arr[target]=scratch[j];
				j++;
			}
		} else {
			if(i<to) {
				arr[target]=scratch[i];
				i++;
			} else {
				arr[target]=scratch[j];
				j++;
			}
		}
		target++;
	}
}

void mergesort(int* arr,unsigned int size) {
	int* scratch=new int[size];
	mergesort(arr,0,size,scratch);
	delete scratch;
}

int main(int argc,char** argv,char** envp) {
	const unsigned int size=10000000;
	int* arr=new int[size];
	for(unsigned int i=0;i<size;i++) {
		arr[i]=random();
	}
	// for debugging...
	//const unsigned int size=4;
	//int arr[size]={ 4,5,7,6 };
	mergesort(arr,size);
	for(unsigned int i=0;i<size;i++) {
		printf("%d: %d\n",i,arr[i]);
	}
	return 0;
}
