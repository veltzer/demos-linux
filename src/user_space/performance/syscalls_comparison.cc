#include <stdio.h> // for printf(3)
#include <sys/time.h> // for gettimeofday(2)
#include <sys/types.h> // for getpid(2), gettid(2)
#include <unistd.h> // for getpid(2)

#include "us_helper.hh" // for micro_diff

/*
 * This demo shows that the performance of various syscalls.
 * we compare:
 * 	- gettimeofdat(2). worst. always does it's thing and needs to read
 * 		the system time.
 * 	- gettid(2). intermediate. not cached.
 * 	- getpid(2). best since it is cached by the kernel.
 *
 * How do I know that gcc actually calls getpid or gettid? I see it in the disassemly.
 * (gettimeofday is obviously called)
 *
 * TODO:
 * - do caching of gettid() using TLS.
 *
 * 		Mark Veltzer
 */

int main(int argc, char **argv, char** envp) {
	struct timeval t1, t2;
	const unsigned int loop=1000000;

	printf("doing %d gettimeofday (for comparison)\n",loop);
	sc(gettimeofday(&t1, NULL));
	for (unsigned int i = 0;i < loop;i++) {
		struct timeval t3;
		gettimeofday(&t3, NULL);
	}
	sc(gettimeofday(&t2, NULL));
	printf("time in micro of one op: %lf\n", micro_diff(&t1,&t2)/(double)loop);

	printf("doing %d getpid\n",loop);
	sc(gettimeofday(&t1, NULL));
	for (unsigned int i = 0;i < loop;i++) {
		getpid();
	}
	sc(gettimeofday(&t2, NULL));
	printf("time in micro of one op: %lf\n", micro_diff(&t1,&t2)/(double)loop);

	printf("doing %d gettid\n",loop);
	sc(gettimeofday(&t1, NULL));
	for (unsigned int i = 0;i < loop;i++) {
		gettid();
	}
	sc(gettimeofday(&t2, NULL));
	printf("time in micro of one op: %lf\n", micro_diff(&t1,&t2)/(double)loop);
	return 0;
}
