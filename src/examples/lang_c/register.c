#include<stdio.h> // for printf(3)

/*
 * An example of doing a loop and telling the compiler that a register should be
 * involved.
 * We will also print the register.
 * How did we know what register the compiler used as the iterator of the loop? We
 * disassembled the code.
 * Note that removing the 'volatile' will cause the code to stop running correctly.
 * (need to look at the disassembly to find out exactly why...).
 * Here is a funny side note: if you remove the printf for i then the compiler
 * will not optimize the loop away because of the printing of the register. But
 * you will see that the value of the register does go down from 100 to 1 (not
 * from 99 to 0 mind you). The compiler thinks that its better to count down
 * than up because of the shorter "compare to zero" at the end of the loop.
 *
 * 		Mark Veltzer
 */

int main(int argc,char** argv,char** envp) {
	unsigned int register i;
	for(i=0;i<100;i++) {
		printf("i is %d\n",i);
		int reg;
		asm volatile("movl %%ebx, %0;":"=r"(reg));
		printf("reg is %d\n",reg);
	}
	return 0;
}
