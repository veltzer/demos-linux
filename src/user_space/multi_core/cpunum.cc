#include <stdio.h> // for printf(3)
#include <unistd.h> // for sysconf(3)

#include "us_helper.hh"

/*
 * This is a simple example showing how to get the number of current cpus which are online...
 * Remmember that this number can change at any minute.
 *
 *		Mark Veltzer
 *
 * EXTRA_LIBS=
 */
int main(int argc, char **argv, char **envp) {
	int ncpus;
	sc(ncpus=sysconf(_SC_NPROCESSORS_ONLN));
	printf("sysconf(_SC_NPROCESSORS_ONLN)=%d\n",ncpus);
	return(0);
}
