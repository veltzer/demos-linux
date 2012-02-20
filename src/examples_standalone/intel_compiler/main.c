#include <stdio.h> // for printf(3)

int main(int argc,char** argv,char** envp) {
	// don't remove the prints, it make the disassembly easier to understand...
	printf("start\n");
	// this is a compiler reordering barrier in the intel compiler...
	// this does not emit any assembly...
	__memory_barrier();
	printf("here\n");
	// this is a machine reordering barrier, which is also a compiler barrier
	// (because the documentation says it is, not because every machine barrier
	// is also a compiler reordering barrier)
	__sync_synchronize();
	printf("end\n");
	return 0;
}
