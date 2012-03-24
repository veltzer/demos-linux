#include <stdio.h>
#include <string.h>

/*
 * This is an example for all the folk wishing to do numerical applications and want to switch
 * between float and double at compile time instead of at run time.
 *
 *		Mark Veltzer
 */

// first option - using floats...
//#define USING_FLOATS
//typedef float some_float;
//#define MY_PRINTF_ARG "%f"

// second option - using doubles...
#define USING_DOUBLES
typedef double some_float;
#define MY_PRINTF_ARG "%lf"

// MC - a macro to make constants
#ifdef USING_FLOATS
#define MC(X) X ## f
#endif
#ifdef USING_DOUBLES
#define MC(X) X
#endif

/*
 * example of a type neutral function
 */
inline some_float add(some_float a, some_float b) {
	return a+b;
}

int main(int argc, char **argv, char **envp) {
	some_float f1=MC(3.14);
	some_float f2=MC(3.14);
	printf(MY_PRINTF_ARG "+" MY_PRINTF_ARG "=" MY_PRINTF_ARG "\n",f1,f2,add(f1,f2));
	return(0);
}
