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
 *
 * TODO:
 * - This example does not compile. Something is wrong with the assembly, I need to fix it.
 *
 * References:
 * https://software.intel.com/en-us/forums/watercooler-catchall/topic/301741
 */

/*
static inline unsigned long getitc(void) {
	unsigned long result;
	// gcc-IA64 version
	__asm__ __volatile__("mov %0=ar.itc" : "=r"(result) :: "memory");
	while (__builtin_expect ((int) result == -1, 0))
		__asm__ __volatile__("mov %0=ar.itc" : "=r"(result) :: "memory");
	return result;
	return 0;
}
*/

static inline unsigned long int getitc(void) {
	//unsigned long int val;
	//__asm__ __volatile__("mov %0=ar.itc" : "=r" (val) :: "memory");
	//__asm__ __volatile__("mov %0,%itc" : "=r" (val) :: "memory");
	//__asm__ __volatile__("mov %0=ar.itc" : "=r" (val) :: "memory");
	//return val;
	return 0;
}

int main(int argc, char** argv, char** envp) {
	printf("itc is %lu\n", getitc());
	CHECK_ZERO(sleep(1));
	printf("itc is %lu\n", getitc());
	return EXIT_SUCCESS;
}
