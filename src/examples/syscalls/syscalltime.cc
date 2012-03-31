#include<stdio.h> // for printf(3)
#include<unistd.h> // for syscall(2), __NR_getpid
#include<sys/syscall.h> // for syscall(2)
#include<sys/time.h> // for gettimeofday(2)

#include"us_helper.hh" // for micro_diff

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
	const unsigned int loop=1000000;
	printf("doing %d syscalls\n",loop);
	gettimeofday(&t1,NULL);
	for(unsigned int i=0;i<loop;i++) {
		struct timeval t3;
		gettimeofday(&t3, NULL);
		//syscall(__NR_getpid);
	}
	gettimeofday(&t2, NULL);
	printf("time in micro of one syscall: %lf\n", micro_diff(&t1,&t2)/(double)loop);
	return EXIT_SUCCESS;
}
