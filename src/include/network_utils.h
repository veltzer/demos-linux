/*
 * This file is part of the linuxapi project.
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

#ifndef __network_utils_h
#define __network_utils_h

#include <firstinclude.h>
#include <us_helper.h>	// for CHECK_NOT_M1()
#include <sys/types.h>	// for open(2)
#include <sys/stat.h>	// for open(2)
#include <fcntl.h>	// for open(2)
#include <unistd.h>	// for read(2), close(2)
#include <stdlib.h>	// for atoi(3)
#include <netdb.h>	// for getservbyname(3)

/* THIS IS A C FILE, NO C++ here */

static inline int get_backlog() {
	// read the data from the /proc/sys/net/core/somaxconn virtual file...
	const char* filename="/proc/sys/net/core/somaxconn";
	const unsigned int size=256;
	char buf[size];
	int fd=CHECK_NOT_M1(open(filename, O_RDONLY));
	CHECK_NOT_M1(read(fd, buf, size));
	CHECK_NOT_M1(close(fd));
	return atoi(buf);
}

static inline void print_servent(struct servent* p_servent) {
	printf("name is %s\n", p_servent->s_name);
	printf("proto is %s\n", p_servent->s_proto);
	printf("port is %d (network order its %d)\n", ntohs(p_servent->s_port), p_servent->s_port);
}

#endif	/* !__network_utils_h */
