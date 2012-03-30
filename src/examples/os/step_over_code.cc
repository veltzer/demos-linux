#include<stdio.h> // for printf(3)
#include<unistd.h> // for getpid(2)

#include "us_helper.hh"

/*
 * This example clearly shows that the code segment is protected against writing.
 * At least on Intel that is.
 *
 * Notes:
 * If you compile this example with optimizations then the final line
 * is necessary because otherwise the compiler will optimize out the entire
 * loop that writes over the code.
 *
 * 	Mark Veltzer
 */

int myfunc(int lim) __attribute__((noinline));
int myfunc(int lim) {
	unsigned int sum=0;
	for(int i=0;i<lim;i++) {
		sum+=i*i;
	}
	return sum;
}

int main(int argc,char** argv,char** envp) {
	printf("sum of square of numbers till 100 is %d\n",myfunc(100));
	printf("my pid is %d\n",getpid());
	my_system("pmap %d",getpid());
	printf("the address of myfunc is %p, look it up above...\n",myfunc);
	waitkey(NULL);
	char* p=(char*)myfunc;
	for(unsigned int i=0;i<10;i++) {
		p[i]=0;
	}
	// do not remove the next call (see above note)
	printf("this is a dummy print which is never reached and must stay here so that the compiler will not optimize away the loop before when compiling with optimization turned on");
	return EXIT_SUCCESS;
}
