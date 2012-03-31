#include<link.h> // for dl_iterate_phdr(3)
#include<stdio.h> // for printf(3)
#include<stdlib.h> // for EXIT_SUCCESS

/*
 * This example shows how to find out which libraries you are linked to.
 * Sort of a cheap ldd replacement.
 *
 * 		Mark Veltzer
 */
static bool verbose=false;

static int callback(struct dl_phdr_info *info, size_t size, void *data) {
	printf("name=%s (%d segments)\n", info->dlpi_name, info->dlpi_phnum);
	if(verbose) {
		for(int j=0;j<info->dlpi_phnum;j++)
			printf("\t\t header %2d: address=%10p\n", j, (void *) (info->dlpi_addr + info->dlpi_phdr[j].p_vaddr));
	}
	return 0;
}

int main(int argc, char** argv,char** envp) {
	dl_iterate_phdr(callback, NULL);
	return EXIT_SUCCESS;
}
