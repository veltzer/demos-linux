#include <stdio.h> // for vprintf(3)
#include <stdarg.h> // for va_start(3), va_arg(3), va_end(3)

/*
 *      This example shows how to use the glibc varargs support to write a var
 *      args function that calls another varargs function.
 *
 *      Notes:
 *      - we use the program_invocation_short_name which is an external name of
 *      our own program. much like argv[0] only we don't need to pass argv until
 *      the point we wish to use it.
 *      - unlike the corresponding builtin va_* compiler internals we can use this
 *      code on any system conforming to the 'stdarg' standard which means this
 *      is quite portable.
 *      - We use the __attribute__ feature to make sure the compiler checks that
 *      we are passing the arguments right (gcc feature).
 *
 *              Mark Veltzer
 */
int trace(const char *fmt, ...) {
	extern char *program_invocation_short_name;

	printf("%s: ", program_invocation_short_name);
	va_list ap;
	va_start(ap,fmt);
	int ret;
	ret = vprintf(fmt, ap);
	va_end(ap);
	return(ret);
}

int trace(const char *fmt, ...) __attribute__((format(printf, 1, 2)));

int main(int argc, char **argv, char **envp) {
	trace("%s %d %f\n", "Hello", 5, 3.14);
	// the next line will produce a compile time error (passing int as string...)
	//trace("%s %s %f\n","Hello",5,3.14);
	return(0);
}
