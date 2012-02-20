#include <stdio.h> // for vprintf(3)
#include <sys/time.h> // for gettimeofday(2)

#include "us_helper.hh" // for micro_diff

/*
 * This is an example of how to use the __restrict gcc feature
 * Note that the keyword "restrict" is not supported by gcc (I am on 4.6.1).
 * It may enter some C++ standard or it may not.
 *
 * What are the semantics of __restrict?
 * It tells the compiler that any access to the values pointed to by this pointer
 * will be done only through this pointer or derived pointers from this pointer.
 *
 * This means that if you write a function that gets two pointers and you add
 * restrict to them then you commit to them having no overlapped areas.
 *
 * Why should you use this? Because it allows the compiler to perform optimizations
 * that it could not do otherwise. In this example the compiler will not need
 * to store the value in *result to main memory and will be able to store
 * it in a register.
 *
 * If you want to see this in action create a disassembled version of this code
 * under different versions of the myrestrict macro below...
 *
 * The difference in performance here is quite noticable...
 *
 *              Mark Veltzer
 */

#define myrestrict __restrict
//#define myrestrict

void add_restrict(int* myrestrict arr,int num, int* myrestrict result) __attribute__ ((noinline));
void add_restrict(int* myrestrict arr,int num, int* myrestrict result) {
	*result=0;
	for(int i=0;i<num;i++) {
		*result+=arr[i];
	}
}

void add_no_restrict(int* arr,int num, int* result) __attribute__ ((noinline));
void add_no_restrict(int* arr,int num, int* result) {
	*result=0;
	for(int i=0;i<num;i++) {
		*result+=arr[i];
	}
}

void add_temp(int* arr,int num, int* result) __attribute__ ((noinline));
void add_temp(int* arr,int num, int* result) {
	int temp=0;
	for(int i=0;i<num;i++) {
		temp+=arr[i];
	}
	*result=temp;
}

int main(int argc, char **argv, char **envp) {
	// prepare a large array
	const unsigned int array_size=1000000;
	int* arr=new int[array_size];
	int res;
	for(unsigned int i=0;i<array_size;i++) {
		arr[i]=random();
	}
	const unsigned int loop=1000;
	struct timeval t1, t2;

	printf("doing %d no restrict calls\n",loop);
	gettimeofday(&t1, NULL);
	for(unsigned int i=0;i<loop;i++) {
		add_no_restrict(arr,array_size,&res);
	}
	gettimeofday(&t2, NULL);
	printf("time in micro of one call: %lf\n", micro_diff(&t1,&t2)/(double)loop);

	printf("doing %d restrict calls\n",loop);
	gettimeofday(&t1, NULL);
	for(unsigned int i=0;i<loop;i++) {
		add_restrict(arr,array_size,&res);
	}
	gettimeofday(&t2, NULL);
	printf("time in micro of one call: %lf\n", micro_diff(&t1,&t2)/(double)loop);

	printf("doing %d temp calls\n",loop);
	gettimeofday(&t1, NULL);
	for(unsigned int i=0;i<loop;i++) {
		add_temp(arr,array_size,&res);
	}
	gettimeofday(&t2, NULL);
	printf("time in micro of one call: %lf\n", micro_diff(&t1,&t2)/(double)loop);
	return(0);
}
