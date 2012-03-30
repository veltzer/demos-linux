#include <stdio.h> // for printf(3)
#include <unistd.h> // for sysconf(3)

#include "us_helper.hh"

/*
 * This is a simple example showing how to get the number of current cpus...
 * _SC_NPROCESSORS_CONF - means configured CPUs, not neccessarily working
 * _SC_NPROCESSORS_ONLN - means online CPUs. You may not have access to all of
 * them.
 * Remmember that the number of online CPUs can change at any minute.
 *
 *		Mark Veltzer
 */
int main(int argc, char **argv, char **envp) {
	int ncpus;
	CHECK_NOT_M1(ncpus=sysconf(_SC_NPROCESSORS_ONLN));
	printf("sysconf(_SC_NPROCESSORS_ONLN)=%d\n",ncpus);
	CHECK_NOT_M1(ncpus=sysconf(_SC_NPROCESSORS_CONF));
	printf("sysconf(_SC_NPROCESSORS_CONF)=%d\n",ncpus);
	return EXIT_SUCCESS;
}
