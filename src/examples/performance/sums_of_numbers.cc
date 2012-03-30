#include<stdio.h> // for printf(3)
#include<stdlib.h> // for EXIT_SUCCESS

/*
 * This example demostrates that the compiler actually adds full loops
 * of numbers at compile time and avoids making the loops at runtime at
 * all... Pretty good! (g++ 4.4.3)
 *
 * Why is this important? Because you may be very confused when debugging
 * an application and seeing that much of the code that you write just isn't
 * there. The wisdom derived here is never underestimate the compilers wisdom
 * to throw your code away or to morph it to something which will be very hard
 * to recognize.
 *
 * Use a disassembler to see the resulting machine code.
 *
 *		Mark Veltzer
 *
 * TODO:
 * - make this example disassemble itself and show what the compiler did, or maybe
 * morph it's own code to show that the sum is there ?
 */
int main(int argc, char **argv, char **envp) {
	int sum;

	sum=0;
	for (int i = 0; i < 100; i++) {
		sum += i;
	}
	printf("sum is %d\n", sum);

	sum=0;
	for (int i = 99; i >= 0; i--) {
		sum += i;
	}
	printf("sum is %d\n", sum);

	sum=0;
	for (int i = 0; i < 100; i+=2) {
		sum += i;
	}
	printf("sum is %d\n", sum);

	sum=0;
	for (int i = 98; i >= 0; i-=2) {
		sum += i;
	}
	printf("sum is %d\n", sum);
	return EXIT_SUCCESS;
}
