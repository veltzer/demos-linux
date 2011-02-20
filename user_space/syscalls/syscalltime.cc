#include <stdio.h> // for printf(3)
#include <unistd.h> // for syscall(2), __NR_getpid
#include <sys/syscall.h> // for syscall(2)
#include <sys/time.h> // for gettimeofday(2)

/*
 * This demo times how long it takes to call a syscall.
 * It is a simple loop surrounded by gettimeofday.
 *
 * How long is it?
 * 	About 1/3 mic on a fast computer.
 * 	About 1.2 mic on my laptop.
 * 	Probably more on ARM processors.
 *
 * 		Mark Veltzer
 */

int main(int argc, char **argv, char** envp) {
	struct timeval t1, t2;
	unsigned long long u1, u2;
	const unsigned int loop=1000000;
	gettimeofday(&t1, NULL);
	for (unsigned int i = 0;i < loop;i++) {
		struct timeval t3;
		gettimeofday(&t3, NULL);
		//syscall(__NR_getpid);
	}
	gettimeofday(&t2, NULL);
	u1=((unsigned long long)t1.tv_sec*1000000)+t1.tv_usec;
	u2=((unsigned long long)t2.tv_sec*1000000)+t2.tv_usec;
	double diff=u2-u1;
	printf("time in micro of one syscall: %lf\n", diff/(double)loop);
	return 0;
}
