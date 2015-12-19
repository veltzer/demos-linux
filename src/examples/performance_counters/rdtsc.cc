/*
 * This file is part of the linuxapi package.
 * Copyright (C) 2011-2015 Mark Veltzer <mark.veltzer@gmail.com>
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
#include <stdio.h>	// for printf(3)
#include <unistd.h>	// for sleep(3)
#include <stdlib.h>	// for EXIT_SUCCESS
#include <err_utils.h>	// for CHECK_ZERO()
#include <lowlevel_utils.h>	// for getrdtsc()

/*
 * Demo for a performance counter on i386...
 */

int main(int argc, char** argv, char** envp) {
	// this is giving me problems on i386 vs x64 (llu vs lu)
	printf("getrdtsc() is %llu\n", getrdtsc());
	CHECK_ZERO(sleep(1));
	// this is giving me problems on i386 vs x64 (llu vs lu)
	printf("getrdtsc() is %llu\n", getrdtsc());
	return EXIT_SUCCESS;
}
