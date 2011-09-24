#include <stdio.h> // for printf(3)
#include <unistd.h> // for syscall(2), __NR_getpid
#include <sys/syscall.h> // for syscall(2)
#include <sys/time.h> // for gettimeofday(2)

#include "us_helper.hh" // for micro_diff

/*
 * This demo shows that the performance of getpid(2) and gettid(2) is good
 * (they are supposed to be cached).
 *
 * TODO:
 * - write this one, the current calls measures gettimeofday(2)
 *
 * 		Mark Veltzer
 */

int main(int argc, char **argv, char** envp) {
	struct timeval t1, t2;
	const unsigned int loop=1000000;
	printf("doing %d gettimeofday\n",loop);
	gettimeofday(&t1, NULL);
	for (unsigned int i = 0;i < loop;i++) {
		struct timeval t3;
		gettimeofday(&t3, NULL);
		//syscall(__NR_getpid);
	}
	gettimeofday(&t2, NULL);
	printf("time in micro of one syscall: %lf\n", micro_diff(&t1,&t2)/(double)loop);
	return 0;
}
