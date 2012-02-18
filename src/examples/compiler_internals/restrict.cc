#include <stdio.h> // for vprintf(3)

/*
 * This is an example of how to use the __restrict gcc feature
 * Note that the keyword "restrict" is not supported by gcc (I am on 4.6.1).
 * It may enter some C++ standard or it may not.
 *
 * What are the semantics of __restrict?
 * It tells the compiler that any access to the values pointed to by this point
 * will be done only through this pointer or derived pointers from this point.
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
 *              Mark Veltzer
 */

#define myrestrict __restrict
//#define myrestrict

void add(int* myrestrict arr,int num, int* myrestrict result) {
	//*result=0;
	for(int i=0;i<num;i++) {
		*result+=arr[i];
	}
}

int main(int argc, char **argv, char **envp) {
	int arr[]={ 1,2,3,4 };
	int res=0;
	add(arr,sizeof(arr)/sizeof(int),&res);
	printf("the sum is %d\n",res);
	return(0);
}
