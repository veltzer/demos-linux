/*
 * This example shows how to find out which libraries you are linked to.
 * Sort of a cheap ldd replacement.
 *
 * 		Mark Veltzer
 */

#include <link.h>
#include <stdlib.h>
#include <stdio.h>

static bool verbose=false;

static int callback(struct dl_phdr_info *info, size_t size, void *data) {
	int j;
	printf("name=%s (%d segments)\n", info->dlpi_name, info->dlpi_phnum);
	if(verbose) {
		for (j = 0; j < info->dlpi_phnum; j++)
			printf("\t\t header %2d: address=%10p\n", j, (void *) (info->dlpi_addr + info->dlpi_phdr[j].p_vaddr));
	}
	return 0;
}

int main(int argc, char *argv[],char** envp) {
	dl_iterate_phdr(callback, NULL);
	return 0;
}
