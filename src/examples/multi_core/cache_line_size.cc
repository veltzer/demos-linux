#include<stdio.h> // for printf(3)
#include<unistd.h> // for sysconf(3)

#include"us_helper.hh"

/*
 * An example showing how to get the cache line size on linux.
 * DCACHE is the data cache.
 * ICACHE is the instruction cache.
 *
 * There are many other parameters you can get via the sysconf interface. Some hardware
 * related and some software. do 'getconf -a' on the cmdline to see them...
 *
 *		Mark Veltzer
 */
int main(int argc, char **argv, char **envp) {
	int linesize;
	CHECK_NOT_M1(linesize=sysconf(_SC_LEVEL1_DCACHE_LINESIZE));
	printf("sysconf(_SC_LEVEL1_DCACHE_LINESIZE)=%d\n",linesize);
	CHECK_NOT_M1(linesize=sysconf(_SC_LEVEL1_ICACHE_LINESIZE));
	printf("sysconf(_SC_LEVEL1_ICACHE_LINESIZE)=%d\n",linesize);
	return EXIT_SUCCESS;
}
