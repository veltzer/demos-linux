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

#include <firstinclude.h>
#include <sys/types.h>	// for stat(2)
#include <sys/stat.h>	// for stat(2)
#include <unistd.h>	// for stat(2)
#include <stdlib.h>	// for EXIT_SUCCESS
#include <stdio.h>	// for printf(3)
#include <us_helper.h>	// for CHECK_NOT_M1(), CHECK_CHARP()
#include <time.h>	// for ctime_r(3)

/*
 * This example explores the time_t type.
 *
 * This shows that on Intel 32 bit time_t is an unsigned integer
 * which is 32 bites wide that counts the seconds from 1/1/1970
 * which means it will run out on:
 * 1970+2^32/(60*60*24*365)=2106
 *
 * TODO:
 * - run this on 64 bit and see the results.
 * - show how to print any format you want from time_t
 */

int main(int argc, char** argv, char** envp) {
	printf("sizeof(time_t) is %d\n", sizeof(time_t));
	struct stat sb;
	CHECK_NOT_M1(stat("/etc/passwd", &sb));
	unsigned int myint=(unsigned int)sb.st_mtime;
	printf("myint is %u\n", myint);
	// lets guess the year according to myint
	printf("the year is %d\n", 1970+myint/(3600*24*365));
	// lets print the time using ctime_r(3)
	// buffer must be at least 26 bytes long (see documentation of ctime(3))
	char buf[30];
	CHECK_CHARP(ctime_r(&sb.st_mtime, buf), buf);
	// no newline on the next line since ctime(3)/ctime_r(3) already have
	// a newline
	printf("ctime_r(&sb.st_mtime, buf) is %s", buf);
	return EXIT_SUCCESS;
}
