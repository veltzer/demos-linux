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

#ifndef __epoll_utils_h
#define __epoll_utils_h

/*
 * This is a helper file for dealing with the linux epoll(2) API
 */

/* THIS IS A C FILE, NO C++ here */

#include <firstinclude.h>
#include <us_helper.h>	// for CHECK_IN_RANGE()
#include <stdio.h>	// for snprintf(3)

static inline void print_events(char* buffer, size_t size, uint32_t events) {
	char* p=buffer;
	int cursize=size;
	if(events & EPOLLIN) {
		size_t ret=CHECK_IN_RANGE(snprintf(p, cursize, "EPOLLIN "), 1, cursize);
		cursize-=ret;
		p+=ret;
	}
	if(events & EPOLLOUT) {
		size_t ret=CHECK_IN_RANGE(snprintf(p, cursize, "EPOLLOUT "), 1, cursize);
		cursize-=ret;
		p+=ret;
	}
	if(events & EPOLLRDHUP) {
		size_t ret=CHECK_IN_RANGE(snprintf(p, cursize, "EPOLLRDHUP "), 1, cursize);
		cursize-=ret;
		p+=ret;
	}
	if(events & EPOLLPRI) {
		size_t ret=CHECK_IN_RANGE(snprintf(p, cursize, "EPOLLPRI "), 1, cursize);
		cursize-=ret;
		p+=ret;
	}
	if(events & EPOLLERR) {
		size_t ret=CHECK_IN_RANGE(snprintf(p, cursize, "EPOLLERR "), 1, cursize);
		cursize-=ret;
		p+=ret;
	}
	if(events & EPOLLHUP) {
		size_t ret=CHECK_IN_RANGE(snprintf(p, cursize, "EPOLLHUP "), 1, cursize);
		cursize-=ret;
		p+=ret;
	}
	if(events & EPOLLET) {
		size_t ret=CHECK_IN_RANGE(snprintf(p, cursize, "EPOLLET "), 1, cursize);
		cursize-=ret;
		p+=ret;
	}
	if(events & EPOLLONESHOT) {
		size_t ret=CHECK_IN_RANGE(snprintf(p, cursize, "EPOLLONESHOT "), 1, cursize);
		cursize-=ret;
		p+=ret;
	}
}

#endif	/* !__epoll_utils_h */
