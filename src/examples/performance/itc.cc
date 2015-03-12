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

/*
 * Demo for a performance counter on i64...
 */

static inline unsigned long getstackpointer(void) {
#if __x86_64
	return 0;
#else
	unsigned long ret;
	asm ("movl %%esp, %0" : "=r" (ret));
	return ret;
#endif	// __x86_64__
}

int main(int argc, char** argv, char** envp) {
	printf("stackpointer is %lu\n", getstackpointer());
	CHECK_ZERO(sleep(1));
	printf("stackpointer is %lu\n", getstackpointer());
	return EXIT_SUCCESS;
}
