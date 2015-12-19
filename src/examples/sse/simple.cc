/*
 * EXTRA_COMPILE_FLAGS=-mmmx -g -march=native
 */

/*
 * This is a simple mmx example. The compiler, if passed the '-mmmx'
 * flag will notice that we are doing vector work and will use
 * an mmx instruction to parallelize the work.
 * see this using:
 * make src/examples/sse/simple.dis
 */

#include <stdio.h>	// for printf(3)
#include <stdlib.h>	// for EXIT_SUCCESS

typedef float v4sf __attribute__((vector_size(16)));

typedef union f4vector {
	v4sf v;
	float f[4];
} f4vector;

int main(int argc, char** argv, char** envp) {
	printf("sizeof(v4sf) is %zd\n", sizeof(v4sf));
	printf("sizeof(f4vector) is %zd\n", sizeof(f4vector));
	f4vector a, b, c;

	a.f[0] = 1; a.f[1] = 2; a.f[2] = 3; a.f[3] = 4;
	b.f[0] = 5; b.f[1] = 6; b.f[2] = 7; b.f[3] = 8;

	c.v = a.v + b.v;
	printf("%f, %f, %f, %f\n", c.f[0], c.f[1], c.f[2], c.f[3]);
	return EXIT_SUCCESS;
}
