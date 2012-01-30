#include <stdio.h> // for vprintf(3)

/*
 * The var args mechanism of C/C++ does not allow one to know exactly how many
 * arguments were passed.
 *
 * For example: the printf function assumes that the user has passed enough arguments
 * who'se type matches the % in the format string but it cannot ascertain it.
 *
 * Can we know how many var args were there quickly? Well, yes we could using a compile
 * time builtin compiler function called '__builtin_va_arg_pack_len'
 * Notes:
 * - the function returns the number of argument with disregard for their type.
 * This allows you to optimize a variadic API to use a non variadic version when that
 * version is not required.
 * - this builtin function can ONLY be used in a function that is GUARANTEED to be
 *   inlined. This is not so with the regular 'inline' keyword. Instead the 
 *   '__attribute__((__gnu_inline__))' or '__attribute__((__always_inline__))'
 *   must be used.
 *
 *              Mark Veltzer
 */
inline  __attribute__((__gnu_inline__)) void trace(int first, ...) {
	int i=__builtin_va_arg_pack_len();
	printf("__builtin_va_arg_pack_len() says %d\n",i);
}

int main(int argc, char **argv, char **envp) {
	int i=7;
	double d=3.14;
	trace(i,i,i);
	trace(1,2,3,4);
	trace(d,d,d,d,d);
	trace(d,d,d,d,d,d,d,d,d);
	return(0);
}
