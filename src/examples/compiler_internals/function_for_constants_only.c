#include <stdio.h> // for printf(3)

/*
 * This example shows how to write a function that only works for constants.
 * This file is a C file because g++ does not have this feature yet.
 * A quick check in the internet shows that a patch to have this in g++ is on the way.
 * Notice that the trick is to create a compile time error and is based on returning
 * a void* instead of an int.
 *
 *              Mark Veltzer
 */
static inline int _sum(int i) {
	return(i *(i + 1) / 2);
}

#define sum(i)\
	__builtin_choose_expr(\
		__builtin_constant_p(i),\
		_sum(i),\
		(void*)0)

/*
 * This inline version does not work because of bug in gcc whereby __builtin_constant_p(i) is not
 * a constant
 */

/*
static __attribute__((always_inline)) int sum(int i) {
	return __builtin_choose_expr(__builtin_constant_p(i),_sum(i),(void*)0);
}
*/

int main(int argc, char **argv, char **envp) {
	printf("sum for 100 is %d\n",sum(100));
	printf("sum for 5 is %d\n",sum(5));
	// these next 2 lines will create a compile time error...
	//volatile int j=25;
	//printf("sum for j is %d\n",sum(j));
	return(0);
}
