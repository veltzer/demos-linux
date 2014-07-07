/*
 * This file is part of the linuxapi package.
 * Copyright (C) 2011-2014 Mark Veltzer <mark.veltzer@gmail.com>
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
#include <unistd.h>	// for getpid(2), syscall(2), sysconf(2), getpagesize(2)
#include <sys/syscall.h>// for syscall(2), SYS_* constants
#include <string.h>	// for strncpy(3), strerror(3)
#include <sys/time.h>	// for getpriority(2)
#include <sys/resource.h>	// for getpriority(2)
#include <pthread.h>	// for the entire pthread_* API
#include <errno.h>	// for errno
#include <stdbool.h>	// for bool
#include <error.h>	// for error_at_line(3)
#include <err.h>// for err(3)
#include <signal.h>	// for sighandler_t
#include <dirent.h>	// for NAME_MAX:const

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

static inline void waitkey(const char *msg) {
	if (msg) {
		fprintf(stdout, "%s...\n", msg);
	} else {
		fprintf(stdout, "Press any key to continue...\n");
	}
	// sc(setvbuf(stdin,NULL,_IONBF,0));
	fgetc(stdin);
}

/*
 * Check that an entire area of memory has a certain value
 */
static inline void memcheck(void *buf, char val, unsigned int size) {
	char *cbuf=(char *)buf;
	unsigned int i;
	for(i=0; i<size; i++) {
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
 * Type struct old_linux_dirent which is taken from the readdir(2)
 * manpage
 */
struct old_linux_dirent {
	long d_ino;	/* inode number */
	off_t d_off;	/* offset to this old_linux_dirent */
	unsigned short d_reclen;/* length of this d_name */
	char d_name[NAME_MAX+1];/* filename (null-terminated) */
};

/*
 * Type struct linux_dirent which is taken from getdents(2) manpage
 */
struct linux_dirent {
	unsigned long d_ino;	/* Inode number */
	unsigned long d_off;	/* Offset to next linux_dirent */
	unsigned short d_reclen;/* Length of this linux_dirent */
	char d_name[];	/* Filename (null-terminated) */
};

/*
 * Wrapper for the getdents(2) system call which is not supplied by glibc
 */
static inline int syscall_getdents(unsigned int fd, struct linux_dirent *dirp, unsigned int count) {
	return syscall(SYS_getdents, fd, dirp, count);
}

/*
 * Wrapper for the readdir(2) system call which is not supplied by glibc
 * This is surrounded by #ifdef because on 64bit I had problems locating
 * the 'SYS_readdir' macro.
 */
#ifdef SYS_readdir
static inline int syscall_readdir(unsigned int fd, struct old_linux_dirent *dirp, unsigned int count) {
	return syscall(SYS_readdir, fd, dirp, count);
}
#endif	/* SYS_readdir */

/*
 * A function that translates an address to it's page boundary
 */
static inline void* page_adr(void* adr) {
	unsigned long iptr=(unsigned long)adr;
	return (void*)(iptr-iptr%getpagesize());
}

#endif	/* !__us_helper_h */
