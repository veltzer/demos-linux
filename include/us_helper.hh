#ifndef __us_helper_h
#define __us_helper_h

#include <iostream>
#include <errno.h>
#include <stdio.h> // for printf(3)
#include <stdlib.h> // for system(3)
#include <stdarg.h>
#include <sys/ioctl.h>
#include <sys/types.h> // for getpid, gettid
#include <sys/syscall.h> // for syscall
#include <unistd.h> // for getpid, syscall, sysconf
#include <cpufreq.h>
#include <proc/readproc.h> // for look_up_our_self(3)

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
		exit(1);
	}
	unsigned long long diff = (t2 - t1) / 1000;
	unsigned long freq = cpufreq_get_freq_kernel(0);
	if(freq==0) {
		fprintf(stderr, "ERROR: freq is 0\n");
		exit(1);
	}
	unsigned long mpart = freq / 1000;
	if(mpart==0) {
		fprintf(stderr, "ERROR: mpart is 0\n");
		exit(1);
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
 */
static inline void scie(int t, const char *msg, int errval = -1) {
	if (t == errval) {
		//throw new std::exception();

		perror("error in system call");
		exit(1);
	}
}
static inline void scassert(int t,const char* msg) {
	if(!t) {
		perror("error in system call");
		exit(1);
	}
}
template<class T> inline void scie(T t,const char *msg, T errval) {
	if (t == errval) {
		throw new std::exception();
	}
}

static inline void scig(int t, const char *msg, int goodval = 0) {
	if (t != goodval) {
		throw new std::exception();

		//perror("error in system call");
		//exit(1);
	}
}

static inline void scig2(int t, const char *msg, int v1, int v2) {
	if ((t != v1) && (t != v2)) {
		throw new std::exception();

		//perror("error in system call");
		//exit(1);
	}
}

static inline void scpe(void *t, const char *msg, void *errval = (void *)-1) {
	if (t == errval) {
		throw new std::exception();

		//perror("error in system call");
		//exit(1);
	}
}
static inline void check_zero(int val,const char* msg) {
	if (val != 0) {
		perror("error in system call");
		exit(1);
	}
}

#define SCIE(v, msg) std::cout << msg << " " << "started" << std::endl; scie(v, msg); std::cout << msg << " " << "ended" << std::endl;
#define SCPE(v, msg) std::cout << msg << " " << "started" << std::endl; scpe(v, msg); std::cout << msg << " " << "ended" << std::endl;
#define SCIG(v, msg) std::cout << msg << " " << "started" << std::endl; scig(v, msg); std::cout << msg << " " << "ended" << std::endl;
#define SCIG2(v, msg, v1, v2) std::cout << msg << " " << "started" << std::endl; scig2(v, msg, v1, v2); std::cout << msg << " " << "ended" << std::endl;

#define SC(v) SCIE(v, __stringify(v));
#define sc(v) scie(v, __stringify(v));

#define CHECK_ZERO(v) check_zero(v, __stringify(v));

// kernel log handling functions
static inline void klog_clear(void) {
	SCIE(system("sudo dmesg -c > /dev/null"), "clearing log");
}

static inline void klog_show(void) {
	SCIE(system("sudo dmesg"), "showing log");
}

static inline void klog_show_clear(void) {
	klog_show();
	klog_clear();
}

static inline void waitkey(const char *msg = NULL) {
	if (msg) {
		fprintf(stdout, "%s...\n", msg);
	} else {
		fprintf(stdout, "Press any key to continue...\n");
	}
	//scie(setvbuf(stdin,NULL,_IONBF,0),"setvbuf");
	fgetc(stdin);
}

static inline void debug(const char *file, const char *function, int line, const char *fmt, ...) {
	extern char *program_invocation_short_name;
	char str[1024];
	pid_t pid = getpid();
	pid_t tid = gettid();

	snprintf(str, 1024, "%s %d/%d %s %s %d: %s\n", program_invocation_short_name, pid, tid, file, function, line, fmt);
	va_list args;
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

	if (filter == NULL) {
		snprintf(cmd, cmd_size, "cat /proc/%d/maps", pid);
	} else {
		snprintf(cmd, cmd_size, "cat /proc/%d/maps | grep %s", pid, filter);
	}
	int res = system(cmd);
	//waitkey();
	return(res);
}

static inline int printbuddy(void) {
	int res = system("cat /proc/buddyinfo");

	//waitkey();
	return(res);
}

static inline void memcheck(void *buf, char val, unsigned int size) {
	char *cbuf = (char *)buf;

	for (unsigned int i = 0; i < size; i++) {
		if (cbuf[i] != val) {
			fprintf(stderr, "ERROR: value at %u is %c and not %c\n", i, cbuf[i], val);
			exit(1);
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

static inline void print_stats(void) {
	static proc_t myproc;
	look_up_our_self(&myproc);
	printf("size is %ld, min_flt is %ld\n",myproc.rss, myproc.min_flt);
}

static inline void print_stats(pid_t pid) {
	static proc_t myproc;
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
	scie(system(str), "system");
}

void my_system(const char *fmt, ...) __attribute__((format(printf, 1, 2)));

#endif // __us_helper_h
