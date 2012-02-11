#include <stdio.h> // for printf(3)
#include <sched.h> // for sched_getcpu(2)
#include <unistd.h> // for sleep(3)
#include <sys/types.h> // for getpid(2)
#include <unistd.h> // for getpid(2)

#include "us_helper.hh"

/*
 * This is an example of a process that prints the CPU it is running on
 * and lets you change the CPU that it is on.
 *
 *		Mark Veltzer
 */
int main(int argc, char **argv, char **envp) {
	pid_t pid=getpid();
	printf("change my cpu using [taskset --cpu-list --pid [list] %d]\n",pid);
	int cpunum;
	while(true) {
		cpunum=sched_getcpu();
		printf("sched_getcpu() says I'm running on core %d\n",cpunum);
		sleep(3);
	}
	return(0);
}
