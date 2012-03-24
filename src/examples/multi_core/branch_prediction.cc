#include <stdio.h> // for printf(3)

// these we stolen shamelessly from the kernel.
// It is a good idea to use them so that if you have a compiler
// that does not have __builtin_expect you would just define likely/unlikely
// in a way that is appropriate to that compiler, or maybe even just
// the identity function.
#define likely(x) __builtin_expect((x),1)
#define unlikely(x) __builtin_expect((x),0)
//#define likely(x) x
//#define unlikely(x) x

/*
 * This is an example of giving hints to the compiler about branchings.
 * Note that:
 * - some cpus have branch prediction in hardware and ignore such hints.
 * - you can pass flags to the compiler to ignore these hints and create
 *	regular branch instructions.
 * - some compiler and/or compiler versions do not actually use this information.
 *
 * Where can this help?
 * It can allow the compiler to reorder the control-flow graph to reduce the number of branches taken for the 'likely' path. This can have a marked improvement in loops where you're checking multiple exit cases.
 *
 * TODO:
 * - on the command line using time(1) this example runs the same with branch prediction and without.
 *	Modify this to have a marked effect on performance and measure itself so that I can show when
 *	this is effective.
 *
 *		Mark Veltzer
 */
int main(int argc, char **argv, char **envp) {
	int x=0;
	long long sum=0;
	//while(likely(x<100000000)) {
	// mispredict on purpose...
	while(unlikely(x<100000000)) {
		// to force the compiler to actually DO the loop.
		// moving this line to sum+=x will actually mean that the compiler
		// will calculate the whole loop in advance!!!
		sum+=x*x;
		x++;
	}
	printf("sum is %lld\n",sum);
	printf("x is %d\n",x);
	return(0);
}
