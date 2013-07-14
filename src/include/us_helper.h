/*
 * This file is part of the linuxapi package.
 * Copyright (C) 2011-2013 Mark Veltzer <mark.veltzer@gmail.com>
 *
 * linuxapi is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * linuxapi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with linuxapi. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __us_helper_h
#define __us_helper_h

/*
 * This is an eclectic collection of helper functions for user space
 * programming. Some of them, after a period of maturation, will ultimately
 * find their resting places in other files. This is sometimes an incubator.
 */

/* THIS IS A C FILE, NO C++ here */

#include <firstinclude.h>
#include <sched.h>	// for sched_getparam(2), sched_getscheduler(2)
#include <cpufreq.h>	// for cpufreq_get_freq_kernel(2)
#include <stdio.h>	// for printf(3), fprintf(3), perror(3), snprintf(3), stdout, stderr, stdin
#include <stdlib.h>	// for system(3), exit(3)
#include <stdarg.h>	// for vsnprintf(3), va_start(3), va_list(3), va_end(3)
#include <sys/types.h>	// for getpid(2), gettid(2)
#include <sys/syscall.h>// for syscall(2)
#include <unistd.h>	// for getpid(2), syscall(2), sysconf(2), getpagesize(2)
#include <string.h>	// for strncpy(3), strerror(3)
#include <sys/time.h>	// for getpriority(2)
#include <sys/resource.h>	// for getpriority(2)
#include <pthread.h>	// for the entire pthread_* API
#include <errno.h>	// for errno
#include <stdbool.h>	// for bool
#include <error.h>	// for error_at_line(3)
#include <err.h>// for err(3)
#include <signal.h>	// for sighandler_t

/*
 * Stringify macros - helps you turn anything into a string
 * Stolen from the linux kernel.
 * This MUST be done in 2 levels...
 * Need to explain how this works...
 */
#define identity(a) a
#define quote(a) # a
#define stringify(x) quote(x)

/*
 * these were stolen shamelessly from the kernel.
 * It is a good idea to use them so that if you have a compiler
 * that does not have __builtin_expect you would just define likely/unlikely
 * in a way that is appropriate to that compiler, or maybe even just
 * the identity function.
 */
#define mylikely(x) __builtin_expect((x), 1)
#define myunlikely(x) __builtin_expect((x), 0)
/*
 * #define mylikely(x) x
 * #define myunlikely(x) x
 */

/*
 * Macro to calculate size of array
 */
#define ARRAY_SIZEOF(a) (sizeof(a)/sizeof(a[0]))
/*
 * Macro to print the sizeof of a type
 */
#define PRINT_SIZEOF(type) printf("size of " stringify(type) " is %zd\n", sizeof(type))

/*
 * getting a thread id (glibc doesnt have this)
 */
static inline pid_t gettid(void) {
	return(syscall(SYS_gettid));
}

static inline unsigned int get_clk_tck(void) {
	return(sysconf(_SC_CLK_TCK));
}

static inline unsigned int min(unsigned int a, unsigned int b) {
	if (a < b) {
		return(a);
	} else {
		return(b);
	}
}

/*
 * Functions which handle the TSC
 *
 * References:
 * http://en.wikipedia.org/wiki/Time_Stamp_Counter
 * http://stackoverflow.com/questions/3388134/rdtsc-accuracy-across-cpu-cores
 */

typedef unsigned long long ticks_t;

static inline ticks_t getticks(void) {
	unsigned int a, d;
	// the volatile here is necessary otherwise the compiler does not know that the value
	// of this register changes and caches it
	// the difference between 'rdtsc' and 'rdtscp' instruction is that the latter will not
	// get executed out of order and therefore measurements using it will be more
	// accurate
	// asm volatile ("rdtsc":"=a" (a), "=d" (d));
	asm volatile ("rdtscp" : "=a" (a), "=d" (d));
	return(((ticks_t)a) | (((ticks_t)d) << 32));
}

static inline unsigned int get_mic_diff(ticks_t t1, ticks_t t2) {
	if (t2 < t1) {
		fprintf(stderr, "ERROR: What's going on? t2<t1...\n");
		exit(EXIT_FAILURE);
	}
	// this is in clicks
	unsigned long long diff=t2 - t1;
	// the frquency returned is in tousands of clicks per second so we multiply
	// unsigned long long freq=cpufreq_get_freq_kernel(0) * 1000;

	// we take the maxiumum frequency for newer Intel machines supporting the
	// 'constant_tsc' feature (see /proc/cpuinfo...).
	unsigned long min, max;
	cpufreq_get_hardware_limits(0, &min, &max);
	unsigned long long freq=max*1000;
	if(freq==0) {
		fprintf(stderr, "ERROR: cpufreq_get_freq_kernel returned 0\n");
		fprintf(stderr, "ERROR: this is probably a problem with your cpu governor setup\n");
		fprintf(stderr, "ERROR: this happens on certain ubuntu systems\n");
		exit(EXIT_FAILURE);
	}
	// how many clicks per micro
	unsigned long long mpart=freq / 1000000;
	if(mpart==0) {
		fprintf(stderr, "ERROR: mpart is 0\n");
		exit(EXIT_FAILURE);
	}
	// how many micros have passed
	unsigned long long mdiff=diff / mpart;
	// fprintf(stdout,"diff is %llu\n",diff);
	// fprintf(stdout,"freq is %llu\n",freq);
	// fprintf(stdout,"mpart is %llu\n",mpart);
	// fprintf(stdout,"mdiff is %llu\n",mdiff);
	return(mdiff);
}

/*
 * A system call handler, will take care of all those pesky error values
 * and will throw an exception if any of them pops up.
 * I removed "throw new std::exception();" from the following functions.
 */
static inline void handle_error(int replace_errno, int new_errno, int usebadval, int badval, const char* msg, const char* file, const char* function, const int line, const char* m) {
	// this is for pthread type errors
	if(replace_errno) {
		errno=new_errno;
	}
	if (m==NULL) {
		if(usebadval) {
			error_at_line(EXIT_FAILURE, errno, file, line, "function is %s, msg is %s, bad value is %d", function, msg, badval);
		} else {
			error_at_line(EXIT_FAILURE, errno, file, line, "function is %s, msg is %s", function, msg);
		}
	} else {
		fprintf(stderr, "%s\n", m);
	}
	// old code follows
	// int save_errno=errno;
	// err(EXIT_FAILURE,"code is %s",msg);
	// fprintf(stderr,"command is %s\n",msg);
	// fprintf(stderr,"location is %s, %s, %d\n",base_file,file,line);
	// if(save_errno!=0) {
	// perror("error in system call");
	// } else {
	// fprintf(stderr,"error: %s\n",strerror(val));
	// }
	exit(EXIT_FAILURE);
}
static inline int check_zero(int val, const char* msg, const char* file, const char* function, const int line, const char* m) {
	if(myunlikely(val!=0)) {
		handle_error(0, 0, 1, val, msg, file, function, line, m);
	}
	return val;
}
static inline int check_zero_errno(int val, const char* msg, const char* file, const char* function, const int line) {
	if(myunlikely(val!=0)) {
		handle_error(1, val, 0, val, msg, file, function, line, NULL);
	}
	return val;
}
static inline int check_not_zero(int val, const char* msg, const char* file, const char* function, const int line) {
	if(myunlikely(val==0)) {
		handle_error(0, 0, 1, val, msg, file, function, line, NULL);
	}
	return val;
}
static inline int check_not_m1(int val, const char* msg, const char* file, const char* function, const int line) {
	if(myunlikely(val==-1)) {
		handle_error(0, 0, 1, val, msg, file, function, line, NULL);
	}
	return val;
}
static inline int check_1(int val, const char* msg, const char* file, const char* function, const int line) {
	if(myunlikely(val!=1)) {
		handle_error(0, 0, 1, val, msg, file, function, line, NULL);
	}
	return val;
}
static inline int check_not_negative(int val, const char* msg, const char* file, const char* function, const int line) {
	if(myunlikely(val<0)) {
		handle_error(0, 0, 1, val, msg, file, function, line, NULL);
	}
	return val;
}
static inline void* check_not_null(void* val, const char* msg, const char* file, const char* function, const int line) {
	if(myunlikely(val==NULL)) {
		handle_error(0, 0, 0, 0, msg, file, function, line, NULL);
	}
	return val;
}
static inline const void* check_not_null_const(const void* val, const char* msg, const char* file, const char* function, const int line) {
	if(myunlikely(val==NULL)) {
		handle_error(0, 0, 0, 0, msg, file, function, line, NULL);
	}
	return val;
}
static inline int check_oneoftwo(int val, const char* msg, int e1, int e2, const char* file, const char* function, const int line) {
	if(myunlikely(val!=e1 && val!=e2)) {
		handle_error(0, 0, 1, val, msg, file, function, line, NULL);
	}
	return val;
}
static inline int check_assert(int val, const char* msg, const char* file, const char* function, const int line) {
	if(myunlikely(!val)) {
		handle_error(0, 0, 1, val, msg, file, function, line, NULL);
	}
	return val;
}
static inline void* check_not_voidp(void* val, const char *msg, void* errval, const char* file, const char* function, const int line) {
	if(myunlikely(val==errval)) {
		handle_error(0, 0, 0, 0, msg, file, function, line, NULL);
	}
	return val;
}
static inline void* check_voidp(void* val, const char *msg, void* errval, const char* file, const char* function, const int line) {
	if(myunlikely(val!=errval)) {
		handle_error(0, 0, 0, 0, msg, file, function, line, NULL);
	}
	return val;
}
static inline sighandler_t check_not_sigt(sighandler_t val, const char *msg, sighandler_t errval, const char* file, const char* function, const int line) {
	if(myunlikely(val==errval)) {
		handle_error(0, 0, 0, 0, msg, file, function, line, NULL);
	}
	return val;
}
static inline int check_int(int val, const char *msg, int expected, const char* file, const char* function, const int line) {
	if(myunlikely(val!=expected)) {
		handle_error(0, 0, 1, val, msg, file, function, line, NULL);
	}
	return val;
}
static inline int check_not_int(int val, const char *msg, int expected, const char* file, const char* function, const int line) {
	if(myunlikely(val==expected)) {
		handle_error(0, 0, 1, val, msg, file, function, line, NULL);
	}
	return val;
}
static inline char* check_charp(char* val, const char *msg, char* expected, const char* file, const char* function, const int line) {
	if(myunlikely(val!=expected)) {
		handle_error(0, 0, 0, 0, msg, file, function, line, NULL);
	}
	return val;
}
static inline int check_in_range(int val, const char *msg, int min, int max, const char* file, const char* function, const int line) {
	if(myunlikely(val<min || val>=max)) {
		handle_error(0, 0, 1, val, msg, file, function, line, NULL);
	}
	return val;
}
static inline int check_positive(int val, const char* msg, const char* file, const char* function, const int line) {
	if(myunlikely(val<=0)) {
		handle_error(0, 0, 1, val, msg, file, function, line, NULL);
	}
	return val;
}
static inline int check_gezero(int val, const char* msg, const char* file, const char* function, const int line) {
	if(myunlikely(val<0)) {
		handle_error(0, 0, 1, val, msg, file, function, line, NULL);
	}
	return val;
}

#define CHECK_ZERO(v) check_zero(v, stringify(v), __FILE__, __FUNCTION__, __LINE__, NULL)
#define CHECK_MSG_ZERO(v, m) check_zero(v, stringify(v), __FILE__, __FUNCTION__, __LINE__, m)
#define CHECK_ZERO_ERRNO(v) check_zero_errno(v, stringify(v), __FILE__, __FUNCTION__, __LINE__)
#define CHECK_NOT_ZERO(v) check_not_zero(v, stringify(v), __FILE__, __FUNCTION__, __LINE__)
#define CHECK_NOT_M1(v) check_not_m1(v, stringify(v), __FILE__, __FUNCTION__, __LINE__)
#define CHECK_1(v) check_1(v, stringify(v), __FILE__, __FUNCTION__, __LINE__)
#define CHECK_NOT_NEGATIVE(v) check_not_negative(v, stringify(v), __FILE__, __FUNCTION__, __LINE__)
#define CHECK_NOT_NULL(v) check_not_null(v, stringify(v), __FILE__, __FUNCTION__, __LINE__)
#define CHECK_NOT_NULL_CONST(v) check_not_null_const(v, stringify(v), __FILE__, __FUNCTION__, __LINE__)
#define CHECK_ONEOFTWO(v, e1, e2) check_oneoftwo(v, stringify(v), e1, e2, __FILE__, __FUNCTION__, __LINE__)
#define CHECK_ASSERT(v) check_assert(v, stringify(v), __FILE__, __FUNCTION__, __LINE__)
#define CHECK_NOT_VOIDP(v, e) check_not_voidp(v, stringify(v), e, __FILE__, __FUNCTION__, __LINE__)
#define CHECK_VOIDP(v, e) check_not_voidp(v, stringify(v), e, __FILE__, __FUNCTION__, __LINE__)
#define CHECK_NOT_SIGT(v, e) check_not_sigt(v, stringify(v), e, __FILE__, __FUNCTION__, __LINE__)
#define CHECK_INT(v, e) check_int(v, stringify(v), e, __FILE__, __FUNCTION__, __LINE__)
#define CHECK_NOT_INT(v, e) check_not_int(v, stringify(v), e, __FILE__, __FUNCTION__, __LINE__)
#define CHECK_CHARP(v, e) check_charp(v, stringify(v), e, __FILE__, __FUNCTION__, __LINE__)
#define CHECK_IN_RANGE(v, min, max) check_in_range(v, stringify(v), min, max, __FILE__, __FUNCTION__, __LINE__)
#define CHECK_POSITIVE(v) check_positive(v, stringify(v), __FILE__, __FUNCTION__, __LINE__)
#define CHECK_GEZERO(v) check_gezero(v, stringify(v), __FILE__, __FUNCTION__, __LINE__)

// kernel log handling functions
static inline void klog_clear(void) {
	CHECK_NOT_M1(system("sudo dmesg -c > /dev/null"));
}

static inline void klog_show(void) {
	CHECK_NOT_M1(system("sudo dmesg"));
}

static inline void klog_show_clear(void) {
	klog_show();
	klog_clear();
}

static inline void waitkey(const char *msg) {
	if (msg) {
		fprintf(stdout, "%s...\n", msg);
	} else {
		fprintf(stdout, "Press any key to continue...\n");
	}
	// sc(setvbuf(stdin,NULL,_IONBF,0));
	fgetc(stdin);
}

static inline void debug(bool short_print, const char *file, const char *function, int line, const char *fmt, ...) {
	extern char *program_invocation_short_name;
	const int BUFSIZE=1024;
	char str[BUFSIZE];
	va_list args;
	pid_t pid=getpid();
	pid_t tid=gettid();
	if(short_print) {
		snprintf(str, BUFSIZE, "%s\n", fmt);
	} else {
		snprintf(str, BUFSIZE, "%s %d/%d %s %s %d: %s\n", program_invocation_short_name, pid, tid, file, function, line, fmt);
	}
	va_start(args, fmt);
	vfprintf(stderr, str, args);
	va_end(args);
}

void debug(bool short_print, const char *file, const char *function, int line, const char *fmt, ...) __attribute__((format(printf, 5, 6)));

/*
 * Semantics of these macros:
 * TRACE - always enabled and always shows max info (usually for debug)
 * DEBUG - cancelled by default and shows max info (turn it on with DO_DEBUG).
 * INFO, WARNING, ERROR, FATAL - doesn't show a lot of info (just the message).
 */
#define TRACE(fmt, args ...) debug(false, __FILE__, __FUNCTION__, __LINE__, fmt, ## args)
#ifdef DO_DEBUG
#define DEBUG(fmt, args ...) debug(false, __FILE__, __FUNCTION__, __LINE__, fmt, ## args)
#else
#define DEBUG(fmt, args ...) do {} while(0)
#endif
#define INFO(fmt, args ...) debug(true, __FILE__, __FUNCTION__, __LINE__, fmt, ## args)
#define WARNING(fmt, args ...) debug(true, __FILE__, __FUNCTION__, __LINE__, fmt, ## args)
#define ERROR(fmt, args ...) debug(true, __FILE__, __FUNCTION__, __LINE__, fmt, ## args)
#define FATAL(fmt, args ...) debug(true, __FILE__, __FUNCTION__, __LINE__, fmt, ## args)

/*
 * Check that an entire area of memory has a certain value
 */
static inline void memcheck(void *buf, char val, unsigned int size) {
	char *cbuf=(char *)buf;
	unsigned int i;
	for (i=0; i<size; i++) {
		if(cbuf[i]!=val) {
			fprintf(stderr, "ERROR: value at %u is %c and not %c\n", i, cbuf[i], val);
			exit(EXIT_FAILURE);
		}
	}
}

/*
 * An easy function to return how many micros have passed between
 * two time vals
 */
static inline double micro_diff(struct timeval* t1, struct timeval* t2) {
	unsigned long long u1, u2;
	u1=((unsigned long long)t1->tv_sec*1000000)+t1->tv_usec;
	u2=((unsigned long long)t2->tv_sec*1000000)+t2->tv_usec;
	double diff=u2-u1;
	return diff;
}

/*
 * Print the scheduling info for the current thread or process
 */
typedef struct _sched_val_and_name {
	int val;
	const char* name;
} sched_val_and_name;

/*
 * List of all schedulers, their names and values
 * see sched_getscheduler(2) for more details about all these constants.
 */
#define entry(x) { x, # x }
static sched_val_and_name sched_tbl[]={
	entry(SCHED_OTHER),
	entry(SCHED_FIFO),
	entry(SCHED_RR),
	entry(SCHED_BATCH),
	entry(SCHED_IDLE),
};
#undef entry

static inline int sched_get_by_name(const char* name) {
	unsigned int i;
	for(i=0; i<ARRAY_SIZEOF(sched_tbl); i++) {
		if(strcmp(name, sched_tbl[i].name)==0) {
			return sched_tbl[i].val;
		}
	}
	CHECK_ASSERT("bad policy name"==NULL);
	return -1;
}

static inline const char* sched_get_name(int policy) {
	unsigned int i;
	for(i=0; i<ARRAY_SIZEOF(sched_tbl); i++) {
		if(sched_tbl[i].val==policy) {
			return sched_tbl[i].name;
		}
	}
	CHECK_ASSERT("bad policy value"==NULL);
	return NULL;
}

static inline void print_scheduling_info() {
	// 0 means the calling thread, process or process group
	// int pri=getpriority(PRIO_PROCESS, 0);
	// printf("getpriority %d\n", tid, pri);
	struct sched_param myparam;
	int scheduler;
	// 0 means current process
	CHECK_NOT_M1(sched_getparam(0, &myparam));
	scheduler=CHECK_NOT_M1(sched_getscheduler(0));
	printf("==================================\n");
	printf("scheduling data for the current thread...\n");
	printf("sched_getparam returned %d\n", myparam.sched_priority);
	printf("sched_getscheduler returned %s\n", sched_get_name(scheduler));
	printf("==================================\n");
}

/*
 * print parameters of the scheduling system
 */
static inline void print_scheduling_consts() {
	unsigned int i;
	for(i=0; i<ARRAY_SIZEOF(sched_tbl); i++) {
		int val=sched_tbl[i].val;
		const char* name=sched_tbl[i].name;
		printf("%s is %d\n", name, val);
	}
}
/*
 * Check that an integer is indeed a policy
 */
static inline void check_policy(int policy) {
	unsigned int i;
	for(i=0; i<ARRAY_SIZEOF(sched_tbl); i++) {
		if(policy==sched_tbl[i].val) {
			return;
		}
	}
	CHECK_ASSERT("bad policy value"==NULL);
}

const int SCHED_FIFO_LOW_PRIORITY=1;
const int SCHED_FIFO_MID_PRIORITY=47;
const int SCHED_FIFO_HIGH_PRIORITY=90;

/*
 * a function to run another function in a high priority thread and wait for it to finish...
 */
static inline void* run_priority(void* (*func)(void*), void* pval, int prio, int policy) {
	check_policy(policy);
	struct sched_param myparam;
	void* retval;
	pthread_attr_t myattr;
	pthread_t mythread;
	if (policy==SCHED_FIFO) {
		myparam.sched_priority=prio;
		pthread_attr_init(&myattr);
		pthread_attr_setinheritsched(&myattr, PTHREAD_EXPLICIT_SCHED);
		pthread_attr_setschedpolicy(&myattr, SCHED_FIFO);
		pthread_attr_setschedparam(&myattr, &myparam);
		CHECK_ZERO_ERRNO(pthread_create(&mythread, &myattr, func, pval));
	} else {
		CHECK_ZERO_ERRNO(pthread_create(&mythread, NULL, func, pval));
	}
	CHECK_ZERO_ERRNO(pthread_join(mythread, &retval));
	return retval;
}

/*
 * A function that checks that no parameters have been passed
 * to an executable.
 */
static inline void no_params(int argc, char** argv) {
	if(argc>1) {
		fprintf(stderr, "%s: usage: %s (without parameters)\n", argv[0], argv[0]);
		exit(EXIT_FAILURE);
	}
}

/*
 * get the current threads stack size
 */
static inline void mypthread_attr_getstack(void** stackaddr, size_t* stacksize) {
	pthread_attr_t at;
	CHECK_ZERO_ERRNO(pthread_getattr_np(pthread_self(), &at));
	CHECK_ZERO_ERRNO(pthread_attr_getstack(&at, stackaddr, stacksize));
}

/*
 * prefault the entire stack for the current thread
 * TODO:
 * - the 0.9 in the next function is a cop-out. Find where we are
 * at the stack, find where the stack begins, calculate how much
 * there is to prefault and prefault it
 */
static inline void stack_prefault() {
	void* stackaddr;
	size_t stacksize;
	mypthread_attr_getstack(&stackaddr, &stacksize);
	// old code
	/* prefault less than the stacksize because when we were called some
	 * of the stack was already used... */
	/*
	 * size_t prefault_size=(int)(stacksize*0.9);
	 * unsigned char dummy[prefault_size];
	 * memset(&dummy, 0, prefault_size);
	 */
	// new code
	int a;
	char* pa=(char*)&a;
	size_t diff=pa-(char*)stackaddr;
	size_t prefault_size=diff-getpagesize();
	unsigned char dummy[prefault_size];
	memset(&dummy, 0, prefault_size);
}

/*
 * A standard max function
 */
static inline int my_max(int a, int b) {
	if(a>b) {
		return a;
	} else {
		return b;
	}
}

/*
 * This function gets the current executables name
 */
extern char* program_invocation_name;
static inline void get_program_name(char* buf, size_t size) {
	snprintf(buf, size, "%s", program_invocation_name);
	// version with /proc
	// ssize_t res=CHECK_NOT_M1(readlink("/proc/self/exe", buf, size));
	// buf[res]='\0';
}

#endif	/* !__us_helper_h */
