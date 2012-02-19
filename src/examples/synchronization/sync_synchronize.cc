#include <stdio.h> // for printf(3)

/*
 * This is a demo to show how sync_synchronize() is implemented...
 * You should see the __sync_synchronize which is a machine memory barrier
 * translated to a "lock orl" instruction in assembly which is an instruction
 * to the core you are running on to stop reading from the cache.
 * So you should see one instruction in the disassembly of this code.
 * And indeed this is what we see.
 *
 * Take note that this is a also a compiler barrier. It prevents the compiler
 * from reordering around this code. You don't see any assembly emitted for
 * that since a compiler barrier is only a compile time instruction about
 * code emittion.
 *
 *              Mark Veltzer
 *
 * EXTRA_LIBS=-lpthread
 */
int main(int argc, char **argv, char **envp) {
	printf("main started\n");
	__sync_synchronize();
	printf("main ended\n");
	return(0);
}
