#include<sys/timerfd.h> // for timerfd_create(2), timerfd_settime(2), timerfd_gettime(2)
#include<unistd.h> // for read(2)
#include<time.h> // for clock_gettime(2)
#include<stdlib.h> // for exit(3)
#include<stdio.h> // for printf(3), fprintf(3), perror(3)
#include<errno.h> // for perror(3)
#include<stdint.h> // for uint64_t

/*
 * This example was stolen shamelessly from the timerfd_create(2) manpage.
 *
 * 	Mark Veltzer
 *
 * EXTRA_LIBS=-lrt
 */

#define handle_error(msg) \
	do { perror(msg); exit(EXIT_FAILURE); } while(0)

static void print_elapsed_time(void) {
	static struct timespec start;
	struct timespec curr;
	static int first_call = 1;
	int secs, nsecs;

	if (first_call) {
		first_call = 0;
		if (clock_gettime(CLOCK_MONOTONIC, &start) == -1)
			handle_error("clock_gettime");
	}
	if (clock_gettime(CLOCK_MONOTONIC, &curr) == -1)
		handle_error("clock_gettime");
	secs = curr.tv_sec - start.tv_sec;
	nsecs = curr.tv_nsec - start.tv_nsec;
	if (nsecs < 0) {
		secs--;
		nsecs += 1000000000;
	}
	printf("%d.%03d: ", secs, (nsecs + 500000) / 1000000);
}

int main(int argc, char** argv,char** envp) {
	struct itimerspec new_value;
	int fd;
	struct timespec now;
	uint64_t exp, tot_exp, max_exp;
	ssize_t s;
	if ((argc != 2) && (argc != 4)) {
		fprintf(stderr, "%s init-secs [interval-secs max-exp]\n",argv[0]);
		exit(EXIT_FAILURE);
	}
	if (clock_gettime(CLOCK_REALTIME, &now) == -1)
		handle_error("clock_gettime");
	/*
	 * Create a CLOCK_REALTIME absolute timer with initial
	 * expiration and interval as specified in command line
	 */
	new_value.it_value.tv_sec = now.tv_sec + atoi(argv[1]);
	new_value.it_value.tv_nsec = now.tv_nsec;
	if (argc == 2) {
		new_value.it_interval.tv_sec = 0;
		max_exp = 1;
	} else {
		new_value.it_interval.tv_sec = atoi(argv[2]);
		max_exp = atoi(argv[3]);
	}
	new_value.it_interval.tv_nsec = 0;

	fd = timerfd_create(CLOCK_REALTIME, 0);
	if (fd == -1)
		handle_error("timerfd_create");
	if (timerfd_settime(fd, TFD_TIMER_ABSTIME, &new_value, NULL) == -1)
		handle_error("timerfd_settime");
	print_elapsed_time();
	printf("timer started\n");
	for(tot_exp=0;tot_exp<max_exp;) {
		s = read(fd, &exp, sizeof(uint64_t));
		if (s != sizeof(uint64_t))
			handle_error("read");
		tot_exp += exp;
		print_elapsed_time();
		printf("read: %llu; total=%llu\n",
			(unsigned long long) exp,
			(unsigned long long) tot_exp);
	}
	return EXIT_SUCCESS;
}
