#include <stdio.h> // for vprintf(3)

/*
 *      This example shows how to use the __builtin_ varargs
 *      of gcc...
 *
 *      Things to notice:
 *      1. We don't need any special header files (we are using compiler built ins
 *      	which are something akin to "sizeof").
 *      2. We use the __attribute__ feature to make sure the compiler checks that
 *      we are passing the arguments right (gcc feature).
 *      3. Notice we can only put the __attribute__ feature on function declaration
 *      (that is why we have the funny declaration after the definition).
 *
 * TODO:
 * - add an example using the glibc wrappers to show the difference.
 *
 *              Mark Veltzer
 *
 * EXTRA_LIBS=
 */
int trace(const char *fmt, ...) {
	extern char *program_invocation_short_name;

	printf("%s: ", program_invocation_short_name);
	__builtin_va_list args;
	int ret;
	__builtin_va_start(args, fmt);
	ret = vprintf(fmt, args);
	__builtin_va_end(args);
	return(ret);
}


int trace(const char *fmt, ...) __attribute__((format(printf, 1, 2)));

int main(int argc, char **argv, char **envp) {
	trace("%s %d %f\n", "Hello", 5, 3.14);
	// the next line will produce a compile time error (passing int as string...)
	//trace("%s %s %f\n","Hello",5,3.14);
	return(0);
}
