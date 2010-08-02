#include <stdio.h>
#include <string.h>

/*
 * This example examines the question of whether it is more efficient to loop
 * from a number down to 0 rather than from 0 to the number. In theory this
 * could have some advantage because comparing to 0 is supposed to be more
 * efficient either in execution or in size of machine command when compared
 * to comparison with an arbitrary value.
 *
 * Compile this code and disassemble it using objdump in order to test this
 * question.
 *
 * The disassembly actually proves that counting down is more efficient than
 * counting up in this specific compiler (g++ 4.4.3) since it saves one machine
 * instruction inside the loop.
 *
 *              Mark Veltzer
 *
 * EXTRA_LIBS=
 */
int main(int argc, char **argv, char **envp) {
	int sum = 0;

	for (int i = 0; i < 100; i++) {
		sum += i * i;
	}
	for (int j = 99; j > 0; j--) {
		sum += j * j;
	}
	printf("sum is %d\n", sum);
	return(0);
}
