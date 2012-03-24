#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

#include "us_helper.hh"

/*
 * This example shows how to use gettimeofday
 * This example also shows how accurate this method of measurement is...
 *
 *		Mark Veltzer
 */
void print_timeval(struct timeval *tv, const char *name) {
	printf("%s - sec is %lu\n", name, tv->tv_sec);
	printf("%s - usec is %lu\n", name, tv->tv_usec);
}

unsigned long diff_timeval_in_micro(struct timeval *tv1, struct timeval *tv2) {
	if (tv1->tv_sec > tv2->tv_sec) {
		fprintf(stderr, "tv1>tv2\n");
		exit(-1);
	} else {
		if (tv1->tv_sec == tv2->tv_sec) {
			if (tv1->tv_usec > tv2->tv_usec) {
				fprintf(stderr, "tv1>tv2\n");
				exit(-1);
			}
		}
	}
	unsigned long diff = (tv2->tv_sec - tv1->tv_sec) * 1000;
	diff += (tv2->tv_usec - tv1->tv_usec) / 1000;
	return(diff);
}

int main(int argc, char **argv, char **envp) {
	struct timeval tv_start, tv_end;

	CHECK_NOT_M1(gettimeofday(&tv_start, NULL));
	usleep(50 * 1000);
	CHECK_NOT_M1(gettimeofday(&tv_end, NULL));
	print_timeval(&tv_start, "start");
	print_timeval(&tv_end, "end");
	unsigned long diff = diff_timeval_in_micro(&tv_start, &tv_end);
	printf("diff is %lu\n", diff);
	return(0);
}
