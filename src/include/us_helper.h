/*
	This file is part of the linuxapi project.
	Copyright (C) 2011, 2012 Mark Veltzer <mark.veltzer@gmail.com>

	The linuxapi package is free software; you can redistribute it and/or
	modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 2.1 of the License, or (at your option) any later version.

	The linuxapi package is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public
	License along with the GNU C Library; if not, write to the Free
	Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
	02111-1307 USA.
*/

#ifndef __us_helper_hh
#define __us_helper_hh

/* THIS IS C FILE, NO C++ here */

#ifndef _GNU_SOURCE
#define _GNU_SOURCE // needed for SCHED_IDLE, SCHED_BATCH
#endif // _GNU_SOURCE
#include<sched.h> // for sched_getparam(2), sched_getscheduler(2)
#include<cpufreq.h> // for cpufreq_get_freq_kernel(2)
#include<sys/prctl.h> // for prctl(2)
#include<stdio.h> // for printf(3), fprintf(3), perror(3), snprintf(3), fflush(3)
#include<stdlib.h> // for system(3), exit(3)
#include<stdarg.h> // for vsnprintf(3), va_start(3), va_list(3), va_end(3)
#include<sys/types.h> // for getpid(2), gettid(2)
#include<sys/syscall.h> // for syscall(2)
#include<unistd.h> // for getpid(2), syscall(2), sysconf(2)
#include<proc/readproc.h> // for get_proc_stats(3)
#include<string.h> // for strncpy(3), strerror(3)
#include<sys/time.h> // for getpriority(2)
#include<sys/resource.h> // for getpriority(2)
#include<pthread.h> // for the entire pthread_* API
#include<errno.h> // for errno
#include<sys/utsname.h> // for uname(2)
#include<stdbool.h> // for bool
#include<signal.h> // for sighandler_t

/*
 * Stringify macros - helps you turn anything into a string
 * Need to explain how this works...
 */
#define __stringify_1(x) # x
#define __stringify(x) __stringify_1(x)

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
 * Functions which handle the RDTSC
 */

typedef unsigned long long ticks_t;

static inline ticks_t getticks(void) {
	unsigned int a, d;
	asm volatile ("rdtsc" : "=a" (a), "=d" (d));
	return(((ticks_t)a) | (((ticks_t)d) << 32));
}

static inline unsigned int get_mic_diff(ticks_t t1, ticks_t t2) {
	if (t2 < t1) {
		fprintf(stderr, "ERROR: What's going on? t2<t1...\n");
		exit(EXIT_FAILURE);
	}
	unsigned long long diff = (t2 - t1) / 1000;
	unsigned long freq = cpufreq_get_freq_kernel(0);
	if(freq==0) {
		fprintf(stderr, "ERROR: cpufreq_get_freq_kernel returned 0\n");
		fprintf(stderr, "ERROR: this is probably a problem with your cpu governor setup\n");
		fprintf(stderr, "ERROR: this happens on certain ubuntu systems\n");
		exit(EXIT_FAILURE);
	}
	unsigned long mpart = freq / 1000;
	if(mpart==0) {
		fprintf(stderr, "ERROR: mpart is 0\n");
		exit(EXIT_FAILURE);
	}
	//unsigned long mdiff=difft/freq;
	unsigned long mdiff = diff / mpart;
	//fprintf(stdout,"diff is %llu\n",diff);
	//fprintf(stdout,"freq is %lu\n",freq);
	//fprintf(stdout,"mpart is %lu\n",mpart);
	//fprintf(stdout,"mdiff is %lu\n",mdiff);
	return(mdiff);
}

/*
 * A system call handler, will take care of all those pesky error values
 * and will throw an exception if any of them pops up.
 * I removed "throw new std::exception();" from the following functions.
 */
// check functions start here
static inline void check_zero(int val,const char* msg,const char* base_file,const char* file,const int line) {
	if (val != 0) {
		fprintf(stderr,"command is %s\n",msg);
		fprintf(stderr,"location is %s, %s, %d\n",base_file,file,line);
		if(errno!=0) {
			perror("error in system call");
		} else {
			fprintf(stderr,"error: %s\n",strerror(val));
		}
		exit(EXIT_FAILURE);
	}
}

static inline void check_not_m1(int val,const char* msg,const char* base_file,const char* file,const int line) {
	if (val ==-1 ) {
		fprintf(stderr,"command is %s\n",msg);
		fprintf(stderr,"location is %s, %s, %d\n",base_file,file,line);
		if(errno!=0) {
			perror("error in system call");
		} else {
			fprintf(stderr,"error: %s\n",strerror(val));
		}
		exit(EXIT_FAILURE);
	}
}
static inline void check_1(int val,const char* msg,const char* base_file,const char* file,const int line) {
	if (val !=1 ) {
		fprintf(stderr,"command is %s\n",msg);
		fprintf(stderr,"location is %s, %s, %d\n",base_file,file,line);
		if(errno!=0) {
			perror("error in system call");
		} else {
			fprintf(stderr,"error: %s\n",strerror(val));
		}
		exit(EXIT_FAILURE);
	}
}
static inline void check_not_negative(int t, const char* msg,const char* base_file,const char* file,const int line) {
	if(t<0) {
		fprintf(stderr,"command is %s\n",msg);
		fprintf(stderr,"location is %s, %s, %d\n",base_file,file,line);
		if(errno!=0) {
			perror("error in system call");
		} else {
			fprintf(stderr,"error: %s\n",strerror(t));
		}
		exit(EXIT_FAILURE);
	}
}
static inline void check_not_null(void* ptr, const char* msg,const char* base_file,const char* file,const int line) {
	if(ptr==NULL) {
		fprintf(stderr,"command is %s\n",msg);
		fprintf(stderr,"location is %s, %s, %d\n",base_file,file,line);
		if(errno!=0) {
			perror("error in system call");
		} else {
			fprintf(stderr,"error: bad pointer\n");
		}
		exit(EXIT_FAILURE);
	}
}
static inline void check_oneoftwo(int v, const char* msg,int e1,int e2,const char* base_file,const char* file,const int line) {
	if(v!=e1 && v!=e2) {
		fprintf(stderr,"command is %s\n",msg);
		fprintf(stderr,"location is %s, %s, %d\n",base_file,file,line);
		if(errno!=0) {
			perror("error in system call");
		} else {
			fprintf(stderr,"error: %s\n",strerror(v));
		}
		exit(EXIT_FAILURE);
	}
}
static inline void check_assert(int t,const char* msg,const char* base_file,const char* file,const int line) {
	if(!t) {
		fprintf(stderr,"command is %s\n",msg);
		fprintf(stderr,"location is %s, %s, %d\n",base_file,file,line);
		perror("error in system call");
		exit(EXIT_FAILURE);
	}
}
static inline void check_not_voidp(void* t,const char *msg, void* errval,const char* base_file,const char* file,const int line) {
	if(t==errval) {
		fprintf(stderr,"command is %s\n",msg);
		fprintf(stderr,"location is %s, %s, %d\n",base_file,file,line);
		perror("error in system call");
		exit(EXIT_FAILURE);
	}
}
static inline void check_not_sigt(sighandler_t t,const char *msg, sighandler_t errval,const char* base_file,const char* file,const int line) {
	if(t==errval) {
		fprintf(stderr,"command is %s\n",msg);
		fprintf(stderr,"location is %s, %s, %d\n",base_file,file,line);
		perror("error in system call");
		exit(EXIT_FAILURE);
	}
}

#define CHECK_ZERO(v) check_zero(v, __stringify(v),__BASE_FILE__,__FUNCTION__,__LINE__);
#define CHECK_NOT_M1(v) check_not_m1(v, __stringify(v),__BASE_FILE__,__FUNCTION__,__LINE__);
#define CHECK_1(v) check_1(v, __stringify(v),__BASE_FILE__,__FUNCTION__,__LINE__);
#define CHECK_NOT_NEGATVIE(v) check_not_negative(v,__stringify(v),__BASE_FILE__,__FUNCTION__,__LINE__)
#define CHECK_NOT_NULL(v) check_not_null(v,__stringify(v),__BASE_FILE__,__FUNCTION__,__LINE__)
#define CHECK_ONEOFTWO(v,e1,e2) check_oneoftwo(v, __stringify(v),e1,e2,__BASE_FILE__,__FUNCTION__,__LINE__);
#define CHECK_ASSERT(v) check_assert(v, __stringify(v),__BASE_FILE__,__FUNCTION__,__LINE__);
#define CHECK_NOT_VOIDP(v,e) check_not_voidp(v, __stringify(v),e,__BASE_FILE__,__FUNCTION__,__LINE__);
#define CHECK_NOT_SIGT(v,e) check_not_sigt(v, __stringify(v),e,__BASE_FILE__,__FUNCTION__,__LINE__);

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
	//sc(setvbuf(stdin,NULL,_IONBF,0));
	fgetc(stdin);
}

static inline void debug(const char *file, const char *function, int line, const char *fmt, ...) {
	extern char *program_invocation_short_name;
	const int BUFSIZE=1024;
	char str[BUFSIZE];
	va_list args;
	pid_t pid = getpid();
	pid_t tid = gettid();

	snprintf(str, BUFSIZE, "%s %d/%d %s %s %d: %s\n", program_invocation_short_name, pid, tid, file, function, line, fmt);
	va_start(args, fmt);
	vfprintf(stderr, str, args);
	va_end(args);
}

void debug(const char *file, const char *function, int line, const char *fmt, ...) __attribute__((format(printf, 4, 5)));

#define DEBUG(fmt, args...) debug(__BASE_FILE__, __FUNCTION__, __LINE__, fmt, ## args)
#define INFO(fmt, args...) debug(__BASE_FILE__, __FUNCTION__, __LINE__, fmt, ## args)
#define TRACE(fmt, args...) debug(__BASE_FILE__, __FUNCTION__, __LINE__, fmt, ## args)

static inline int printproc(const char *filter) {
	pid_t pid = getpid();
	const unsigned int cmd_size = 256;
	char cmd[cmd_size];
	int res;

	if (filter == NULL) {
		snprintf(cmd, cmd_size, "cat /proc/%d/maps", pid);
	} else {
		snprintf(cmd, cmd_size, "cat /proc/%d/maps | grep %s", pid, filter);
	}
	res=system(cmd);
	//waitkey();
	return(res);
}

static inline void memcheck(void *buf, char val, unsigned int size) {
	char *cbuf = (char *)buf;
	unsigned int i;

	for (i = 0; i < size; i++) {
		if (cbuf[i] != val) {
			fprintf(stderr, "ERROR: value at %u is %c and not %c\n", i, cbuf[i], val);
			exit(EXIT_FAILURE);
		}
	}
}

// progress handling functions...
static inline void do_prog_init(void) {
	printf("progress: \n");
	fflush(stdout);
}

static inline void do_prog(unsigned int i, unsigned int mod, unsigned int full) {
	if (i % mod == 0) {
		printf("\r\t%d/%d", i, full);
		fflush(stdout);
	}
}

static inline void do_prog_finish(void) {
	printf("\tfinished...\n");
	fflush(stdout);
}

static inline void print_stats(pid_t pid) {
	proc_t myproc;
	get_proc_stats(pid,&myproc);
	printf("size is %ld, min_flt is %ld, state is %c\n",myproc.rss, myproc.min_flt, myproc.state);
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
	vsnprintf(str, cmd_size,fmt, args);
	va_end(args);
	fprintf(stderr,"doing [%s]\n",str);
	CHECK_NOT_M1(system(str));
}

void my_system(const char *fmt, ...) __attribute__((format(printf, 1, 2)));

static inline void print_process_name(void) {
	const unsigned int size=16;
	char name[size];
	CHECK_ZERO(prctl(PR_GET_NAME,name));
	TRACE("process name is [%s]",name);
}

static inline void get_process_name(char* buffer,unsigned int bufsize) {
	const unsigned int size=16;
	char name[size];
	CHECK_ZERO(prctl(PR_GET_NAME,name));
	strncpy(buffer,name,bufsize);
}

static inline void set_process_name(const char* newname) {
	const unsigned int size=16;
	char name[size];
	strncpy(name,newname,size);
	CHECK_ZERO(prctl(PR_SET_NAME,name));
}

static inline void print_process_name_from_proc(void) {
	my_system("cat /proc/%d/comm",getpid());
}

static inline void printbuddy(void) {
	my_system("cat /proc/buddyinfo");
}

static inline double micro_diff(struct timeval* t1,struct timeval* t2) {
	unsigned long long u1,u2;
	u1=((unsigned long long)t1->tv_sec*1000000)+t1->tv_usec;
	u2=((unsigned long long)t2->tv_sec*1000000)+t2->tv_usec;
	double diff=u2-u1;
	return diff;
}
static const char* schedulers[]={
	"SCHED_OTHER",
	"SCHED_FIFO",
	"SCHED_RR",
	"SCHED_BATCH",
	"SCHED_IDLE",
};
static inline void print_scheduling_info() {
	// 0 means the calling thread, process or process group
	//int pri = getpriority(PRIO_PROCESS, 0);
	//printf("getpriority %d\n", tid, pri);
	struct sched_param myparam;
	int scheduler;
	// 0 means current process
	CHECK_NOT_M1(sched_getparam(0,&myparam));
	CHECK_NOT_M1(scheduler=sched_getscheduler(0));
	printf("==================================\n");
	printf("scheduling data for the current thread...\n");
	printf("sched_getparam returned %d\n", myparam.sched_priority);
	printf("sched_getscheduler returned %s\n", schedulers[scheduler]);
	printf("==================================\n");
}

static inline void print_scheduling_consts() {
	printf("SCHED_OTHER is %d\n",SCHED_OTHER);
	printf("SCHED_FIFO is %d\n",SCHED_FIFO);
	printf("SCHED_RR is %d\n",SCHED_RR);
	//printf("SCHED_BATCH is %d\n",SCHED_BATCH);
	//printf("SCHED_IDLE is %d\n",SCHED_IDLE);
}

// a function to run another function in a high priority thread and wait for it to finish...
// TODO: error checking in this function is bad.
static inline void* run_high_priority(void* (*func)(void*),void* val,int prio) {
	int res;
	struct sched_param myparam;
	void* retval;
	pthread_attr_t myattr;
	pthread_t mythread;
	//myparam.sched_priority = sched_get_priority_max(SCHED_FIFO);
	myparam.sched_priority = prio;
	pthread_attr_init(&myattr);
	pthread_attr_setinheritsched(&myattr, PTHREAD_EXPLICIT_SCHED);
	pthread_attr_setschedpolicy(&myattr, SCHED_FIFO);
	pthread_attr_setschedparam(&myattr, &myparam);
	res=pthread_create(&mythread, &myattr, func, val);
	if(res) {
		errno=res;
		perror("pthread_create");
		printf("trying to create thread with prio %d failed, check ulimit -a...\n",myparam.sched_priority);
		exit(EXIT_FAILURE);
	}
	pthread_join(mythread, &retval);
	return retval;
	//printf("thread joined and return val was %p\n",retval);
}

static inline void print_cpu_set(FILE* pfile,cpu_set_t *p) {
	int j;
	fprintf(pfile, "CPU_COUNT is %d\n", CPU_COUNT(p));
	fprintf(pfile, "CPU_SETSIZE is %d\n", CPU_SETSIZE);
	for (j = 0; j < CPU_SETSIZE; j++) {
		if (CPU_ISSET(j, p)) {
			fprintf(pfile,"\tCPU %d\n", j);
		}
	}
}

#endif /* !__us_helper_hh */
