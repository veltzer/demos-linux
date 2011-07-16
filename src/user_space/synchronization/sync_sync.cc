#include <stdio.h> // for printf(3)

/*
 * This is a demo to show how sync_synchronize() is implemented...
 * Actually - you shouldn't see any assembly emitted here since this
 * is a compiler barrier and not a memory barrier.
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
