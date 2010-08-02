#include <stdio.h>
#include <string.h>

/*
 * This example demostrates that the compiler actually adds full loops
 * of numbers at compile time and avoids making the loops at runtime at
 * all... Pretty good! (g++ 4.4.3)
 *
 * Use a disassembler to see the resulting machine code...
 *
 *              Mark Veltzer
 *
 * EXTRA_LIBS=
 */
int main(int argc, char **argv, char **envp) {
	int sum = 0;

	for (int i = 0; i < 100; i++) {
		sum += i;
	}
	for (int j = 99; j > 0; j--) {
		sum += j;
	}
	printf("sum is %d\n", sum);
	return(0);
}
