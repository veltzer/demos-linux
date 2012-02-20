#include <stdio.h> // for printf(3)

int main(int argc,char** argv,char** envp) {
	// don't remove the prints, it make the disassembly easier to understand...
	printf("===================================\n");

	// this is a compiler reordering barrier in the intel compiler...
	// this does not emit any assembly...
	__memory_barrier();

	printf("===================================\n");

	// this is a machine reordering barrier, which is also a compiler barrier
	// (because the documentation says it is, not because every machine barrier
	// is also a compiler reordering barrier)
	__sync_synchronize();

	printf("===================================\n");

	// here is a compare and swap that succeeds...
	int a=4;
	int old_val=__sync_val_compare_and_swap(&a,4,5);
	printf("old_val is %d and a is %d\n",old_val,a);

	printf("===================================\n");

	// here is a compare and swap that fails...
	a=17;
	old_val=__sync_val_compare_and_swap(&a,4,5);
	printf("old_val is %d and a is %d\n",old_val,a);

	printf("===================================\n");
	return 0;
}
