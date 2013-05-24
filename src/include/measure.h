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

#ifndef __measure_h
#define __measure_h

/* THIS IS A C FILE, NO C++ here */

#include <stdio.h> // for printf(3)
#include <sys/time.h> // for gettimeofday(2), struct timeval
#include <us_helper.h> // for micro_diff()

typedef struct _measure {
	struct timeval t1;
	struct timeval t2;
} measure;

static inline void measure_start(measure* m,const char* msg) {
	printf("measure start: [%s]\n",msg);
	gettimeofday(&m->t1, NULL);
}

static inline void measure_end(measure* m,const char* msg) {
	gettimeofday(&m->t2, NULL);
	printf("measure end: [%s]\n",msg);
}

static inline void measure_print(measure* m,const char* msg,int attempts) {
	printf("measure print: time in micro of single [%s]: %lf\n", msg, micro_diff(&m->t1, &m->t2)/(double)attempts);
}

#endif	/* !__measure_h */
