#include <stdio.h> // for printf(3)
#include <stdarg.h> // for va_start(3), va_arg(3), va_end(3)

/*
 * This example shows how to code a function that receives a variable
 * number of arguments and iterates them.
 *
 *		Mark Veltzer
 *
 * EXTRA_LIBS=
 */
void trace(const char *fmt, ...) {
	va_list ap;
	int d;
	char c, *s;
	va_start(ap,fmt);
	while (*fmt) {
		switch (*fmt++) {
			case 's': /* string */
				s = va_arg(ap, char *);
				printf("string %s\n", s);
				break;
			case 'd': /* int */
				d = va_arg(ap, int);
				printf("int %d\n", d);
				break;
			case 'c': /* char */
			/* need a cast here since va_arg only
			* takes fully promoted types */
				c = va_arg(ap, char);
				printf("char %c\n", c);
				break;
		}
	}
	va_end(ap);
}

int trace(const char *fmt, ...) __attribute__((format(printf, 1, 2)));

int main(int argc, char **argv, char **envp) {
	trace("%s %d %f\n", "Hello", 5, 3.14);
	// the next line will produce a compile time error (passing int as string...)
	//trace("%s %s %f\n","Hello",5,3.14);
	return(0);
}
