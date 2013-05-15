/*
 * This file is part of the linuxapi project.
 * Copyright (C) 2011-2013 Mark Veltzer <mark.veltzer@gmail.com>
 *
 * The linuxapi package is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * The linuxapi package is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with the GNU C Library; if not, write to the Free
 * Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 * 02111-1307 USA.
 */

#ifndef __us_helper_h
#define __us_helper_h

/* THIS IS A C FILE, NO C++ here */

#include <firstinclude.h>
#include <sched.h>	// for sched_getparam(2), sched_getscheduler(2), CPU_COUNT(3)
#include <cpufreq.h>	// for cpufreq_get_freq_kernel(2)
#include <sys/prctl.h>	// for prctl(2)
#include <stdio.h>	// for printf(3), fprintf(3), perror(3), snprintf(3), fflush(3)
#include <stdlib.h>	// for system(3), exit(3)
#include <stdarg.h>	// for vsnprintf(3), va_start(3), va_list(3), va_end(3)
#include <sys/types.h>	// for getpid(2), gettid(2)
#include <sys/syscall.h>// for syscall(2)
#include <unistd.h>	// for getpid(2), syscall(2), sysconf(2)
#include <proc/readproc.h>	// for get_proc_stats(3)
#include <string.h>	// for strncpy(3), strerror(3)
#include <sys/time.h>	// for getpriority(2)
#include <sys/resource.h>	// for getpriority(2)
#include <pthread.h>	// for the entire pthread_* API
#include <errno.h>	// for errno
#include <sys/utsname.h>// for uname(2)
#include <stdbool.h>	// for bool
#include <signal.h>	// for sighandler_t, sigaction(2)
#include <error.h>	// for error_at_line(3)
#include <err.h>// for err(3)

/*
 * Stringify macros - helps you turn anything into a string
 * Need to explain how this works...
 */
#define __stringify_1(x) # x
#define __stringify(x) __stringify_1(x)

/*
 * these we stolen shamelessly from the kernel.
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
static inline void handle_error(int val, const char* msg, const char* file, const char* base_file, const char* function, const int line) {
	// this is for pthread type errors
	if(val!=0) {
		errno=val;
	}
	error_at_line(EXIT_FAILURE, errno, base_file, line, "function is %s, code is %s", function, msg);

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
	// exit(EXIT_FAILURE);
}
static inline int check_zero(int val, const char* msg, const char* file, const char* base_file, const char* function, const int line) {
	if(myunlikely(val!=0)) {
		handle_error(val, msg, file, base_file, function, line);
	}
	return val;
}
static inline int check_not_zero(int val, const char* msg, const char* file, const char* base_file, const char* function, const int line) {
	if(myunlikely(val==0)) {
		handle_error(val, msg, file, base_file, function, line);
	}
	return val;
}
static inline int check_not_m1(int val, const char* msg, const char* file, const char* base_file, const char* function, const int line) {
	if(myunlikely(val==-1)) {
		handle_error(val, msg, file, base_file, function, line);
	}
	return val;
}
static inline int check_1(int val, const char* msg, const char* file, const char* base_file, const char* function, const int line) {
	if(myunlikely(val!=1)) {
		handle_error(val, msg, file, base_file, function, line);
	}
	return val;
}
static inline int check_not_negative(int val, const char* msg, const char* file, const char* base_file, const char* function, const int line) {
	if(myunlikely(val<0)) {
		handle_error(val, msg, file, base_file, function, line);
	}
	return val;
}
static inline void* check_not_null(void* val, const char* msg, const char* file, const char* base_file, const char* function, const int line) {
	if(myunlikely(val==NULL)) {
		handle_error(0, msg, file, base_file, function, line);
	}
	return val;
}
static inline int check_oneoftwo(int val, const char* msg, int e1, int e2, const char* file, const char* base_file, const char* function, const int line) {
	if(myunlikely(val!=e1 && val!=e2)) {
		handle_error(val, msg, file, base_file, function, line);
	}
	return val;
}
static inline int check_assert(int val, const char* msg, const char* file, const char* base_file, const char* function, const int line) {
	if(myunlikely(!val)) {
		handle_error(val, msg, file, base_file, function, line);
	}
	return val;
}
static inline void* check_not_voidp(void* val, const char *msg, void* errval, const char* file, const char* base_file, const char* function, const int line) {
	if(myunlikely(val==errval)) {
		handle_error(0, msg, file, base_file, function, line);
	}
	return val;
}
static inline void* check_voidp(void* val, const char *msg, void* errval, const char* file, const char* base_file, const char* function, const int line) {
	if(myunlikely(val!=errval)) {
		handle_error(0, msg, file, base_file, function, line);
	}
	return val;
}
static inline sighandler_t check_not_sigt(sighandler_t val, const char *msg, sighandler_t errval, const char* file, const char* base_file, const char* function, const int line) {
	if(myunlikely(val==errval)) {
		handle_error(0, msg, file, base_file, function, line);
	}
	return val;
}
static inline int check_int(int val, const char *msg, int expected, const char* file, const char* base_file, const char* function, const int line) {
	if(myunlikely(val!=expected)) {
		handle_error(val, msg, file, base_file, function, line);
	}
	return val;
}
static inline char* check_charp(char* val, const char *msg, char* expected, const char* file, const char* base_file, const char* function, const int line) {
	if(myunlikely(val!=expected)) {
		handle_error(0, msg, file, base_file, function, line);
	}
	return val;
}
static inline int check_in_range(int val, const char *msg, int min, int max, const char* file, const char* base_file, const char* function, const int line) {
	if(myunlikely(val<min || val>=max)) {
		fprintf(stderr, "val is %d, min is %d, max is %d\n", val, min, max);
		handle_error(val, msg, file, base_file, function, line);
	}
	return val;
}
static inline int check_positive(int val, const char* msg, const char* file, const char* base_file, const char* function, const int line) {
	if(myunlikely(val<=0)) {
		handle_error(val, msg, file, base_file, function, line);
	}
	return val;
}
static inline int check_gezero(int val, const char* msg, const char* file, const char* base_file, const char* function, const int line) {
	if(myunlikely(val<0)) {
		handle_error(val, msg, file, base_file, function, line);
	}
	return val;
}

#define CHECK_ZERO(v) check_zero(v, __stringify(v), __FILE__, __BASE_FILE__, __FUNCTION__, __LINE__)
#define CHECK_NOT_ZERO(v) check_not_zero(v, __stringify(v), __FILE__, __BASE_FILE__, __FUNCTION__, __LINE__)
#define CHECK_NOT_M1(v) check_not_m1(v, __stringify(v), __FILE__, __BASE_FILE__, __FUNCTION__, __LINE__)
#define CHECK_1(v) check_1(v, __stringify(v), __FILE__, __BASE_FILE__, __FUNCTION__, __LINE__)
#define CHECK_NOT_NEGATIVE(v) check_not_negative(v, __stringify(v), __FILE__, __BASE_FILE__, __FUNCTION__, __LINE__)
#define CHECK_NOT_NULL(v) check_not_null(v, __stringify(v), __FILE__, __BASE_FILE__, __FUNCTION__, __LINE__)
#define CHECK_ONEOFTWO(v, e1, e2) check_oneoftwo(v, __stringify(v), e1, e2, __FILE__, __BASE_FILE__, __FUNCTION__, __LINE__)
#define CHECK_ASSERT(v) check_assert(v, __stringify(v), __FILE__, __BASE_FILE__, __FUNCTION__, __LINE__)
#define CHECK_NOT_VOIDP(v, e) check_not_voidp(v, __stringify(v), e, __FILE__, __BASE_FILE__, __FUNCTION__, __LINE__)
#define CHECK_VOIDP(v, e) check_not_voidp(v, __stringify(v), e, __FILE__, __BASE_FILE__, __FUNCTION__, __LINE__)
#define CHECK_NOT_SIGT(v, e) check_not_sigt(v, __stringify(v), e, __FILE__, __BASE_FILE__, __FUNCTION__, __LINE__)
#define CHECK_INT(v, e) check_int(v, __stringify(v), e, __FILE__, __BASE_FILE__, __FUNCTION__, __LINE__)
#define CHECK_CHARP(v, e) check_charp(v, __stringify(v), e, __FILE__, __BASE_FILE__, __FUNCTION__, __LINE__)
#define CHECK_IN_RANGE(v, min, max) check_in_range(v, __stringify(v), min, max, __FILE__, __BASE_FILE__, __FUNCTION__, __LINE__)
#define CHECK_POSITIVE(v) check_positive(v, __stringify(v), __FILE__, __BASE_FILE__, __FUNCTION__, __LINE__)
#define CHECK_GEZERO(v) check_gezero(v, __stringify(v), __FILE__, __BASE_FILE__, __FUNCTION__, __LINE__)

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

#define TRACE(fmt, args ...) debug(false, __BASE_FILE__, __FUNCTION__, __LINE__, fmt, ## args)
#define DEBUG(fmt, args ...) debug(false, __BASE_FILE__, __FUNCTION__, __LINE__, fmt, ## args)
#define INFO(fmt, args ...) debug(true, __BASE_FILE__, __FUNCTION__, __LINE__, fmt, ## args)
#define WARNING(fmt, args ...) debug(true, __BASE_FILE__, __FUNCTION__, __LINE__, fmt, ## args)
#define ERROR(fmt, args ...) debug(true, __BASE_FILE__, __FUNCTION__, __LINE__, fmt, ## args)
#define FATAL(fmt, args ...) debug(true, __BASE_FILE__, __FUNCTION__, __LINE__, fmt, ## args)

static inline int printproc(const char *filter) {
	pid_t pid=getpid();
	const unsigned int cmd_size=256;
	char cmd[cmd_size];
	int res;
	if (filter==NULL) {
		snprintf(cmd, cmd_size, "cat /proc/%d/maps", pid);
	} else {
		snprintf(cmd, cmd_size, "cat /proc/%d/maps | grep %s", pid, filter);
	}
	res=system(cmd);
	// waitkey();
	return(res);
}

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
 * progress handling functions...
 */
static inline void do_prog_init(void) {
	printf("progress: \n");
	fflush(stdout);
}

static inline void do_prog(unsigned int i, unsigned int mod, unsigned int full) {
	if (i % mod==0) {
		printf("\r\t%d/%d", i, full);
		fflush(stdout);
	}
}

static inline void do_prog_finish(void) {
	printf("\tfinished...\n");
	fflush(stdout);
}

/*
 * Print memory stats for the current process
 */
static inline void print_stats(pid_t pid) {
	proc_t myproc;
	get_proc_stats(pid, &myproc);
	printf("size is %ld, min_flt is %ld, state is %c\n", myproc.rss, myproc.min_flt, myproc.state);
}

/*
 * An enhanced system(3) version which also:
 * - accepts variable argument and does the substitution.
 * - checks for errors on return from system(3)
 */
static inline void my_system(const char *fmt, ...) {
	const unsigned int cmd_size=1024;
	char str[cmd_size];
	va_list args;

	va_start(args, fmt);
	vsnprintf(str, cmd_size, fmt, args);
	va_end(args);
	fprintf(stderr, "doing [%s]\n", str);
	CHECK_NOT_M1(system(str));
}

void my_system(const char *fmt, ...) __attribute__((format(printf, 1, 2)));

/*
 * Handle process names
 */
static inline void print_process_name(void) {
	const unsigned int size=16;
	char name[size];
	CHECK_ZERO(prctl(PR_GET_NAME, name));
	TRACE("process name is [%s]", name);
}

static inline void get_process_name(char* buffer, unsigned int bufsize) {
	const unsigned int size=16;
	char name[size];
	CHECK_ZERO(prctl(PR_GET_NAME, name));
	strncpy(buffer, name, bufsize);
}

static inline void set_process_name(const char* newname) {
	const unsigned int size=16;
	char name[size];
	strncpy(name, newname, size);
	CHECK_ZERO(prctl(PR_SET_NAME, name));
}

static inline void print_process_name_from_proc(void) {
	my_system("cat /proc/%d/comm", getpid());
}

/*
 * Print kernel memory buddy info from /proc
 */
static inline void printbuddy(void) {
	my_system("cat /proc/buddyinfo");
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
static const char* schedulers[]={
	"SCHED_OTHER",
	"SCHED_FIFO",
	"SCHED_RR",
	"SCHED_BATCH",
	"SCHED_IDLE",
};

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
	printf("sched_getscheduler returned %s\n", schedulers[scheduler]);
	printf("==================================\n");
}

/*
 * print parameters of the scheduling system
 */
static inline void print_scheduling_consts() {
	printf("SCHED_OTHER is %d\n", SCHED_OTHER);
	printf("SCHED_FIFO is %d\n", SCHED_FIFO);
	printf("SCHED_RR is %d\n", SCHED_RR);
	printf("SCHED_BATCH is %d\n", SCHED_BATCH);
	printf("SCHED_IDLE is %d\n", SCHED_IDLE);
}

/*
 * a function to run another function in a high priority thread and wait for it to finish...
 */
static inline void* run_high_priority(void* (*func)(void*), void* pval, int prio) {
	struct sched_param myparam;
	void* retval;
	pthread_attr_t myattr;
	pthread_t mythread;
	myparam.sched_priority=prio;
	pthread_attr_init(&myattr);
	pthread_attr_setinheritsched(&myattr, PTHREAD_EXPLICIT_SCHED);
	pthread_attr_setschedpolicy(&myattr, SCHED_FIFO);
	pthread_attr_setschedparam(&myattr, &myparam);
	CHECK_ZERO(pthread_create(&mythread, &myattr, func, pval));
	CHECK_ZERO(pthread_join(mythread, &retval));
	return retval;
}
const int STANDARD_HIGH_PRIORITY=90;

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
 * A function to print cpu sets
 */
static inline void print_cpu_set(FILE* pfile, cpu_set_t *p) {
	int j;
	fprintf(pfile, "CPU_COUNT is %d\n", CPU_COUNT(p));
	fprintf(pfile, "CPU_SETSIZE is %d\n", CPU_SETSIZE);
	for (j=0; j < CPU_SETSIZE; j++) {
		if (CPU_ISSET(j, p)) {
			fprintf(pfile, "\tCPU %d\n", j);
		}
	}
}

/*
 * easy registration of signals via signal(2)
 */
typedef void (*sig_old_handler)(int);
void register_handler_signal(int signum, sig_old_handler handler) {
	CHECK_NOT_SIGT(signal(signum, handler), SIG_ERR);
}

/*
 * easy registration of signals via sigaction(2)
 */
typedef void (*sig_handler)(int, siginfo_t *, void *);
static inline void register_handler_sigaction(int sig, sig_handler handler) {
	struct sigaction sa;
	sa.sa_flags=SA_SIGINFO;
	sigemptyset(&sa.sa_mask);
	sa.sa_sigaction=handler;
	CHECK_NOT_M1(sigaction(sig, &sa, NULL));
}

/*
 * get the current threads stack size
 */
static inline size_t pthread_get_current_stack_size() {
	pthread_attr_t at;
	void* stackaddr;
	size_t stacksize;
	CHECK_ZERO(pthread_getattr_np(pthread_self(), &at));
	CHECK_ZERO(pthread_attr_getstack(&at, &stackaddr, &stacksize));
	return stacksize;
}

/*
 * prefault the entire stack for the current thread
 */
static inline void stack_prefault() {
	size_t stacksize=pthread_get_current_stack_size();
	/* prefault less than the stacksize because when we were called some
	 * of the stack was already used... */
	size_t prefault_size=(int)(stacksize*0.9);
	unsigned char dummy[prefault_size];
	memset(&dummy, 0, prefault_size);
}

#endif	/* !__us_helper_h */
