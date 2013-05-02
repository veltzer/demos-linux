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

#include <firstinclude.h>
#include <stdlib.h>	// for EXIT_SUCCESS, EXIT_FAILURE
#include <sys/time.h>	// for gettimeofday(2)
#include <us_helper.h>	// for TRACE(), micro_diff, CHECK_ZERO

/*
 * This example is written so that you could disassemble the machine
 * code for volatiles and atomics. That is also why debug info is enabled
 * for it (so that you could see the source code interleaved with the
 * assembly).
 * EXTRA_COMPILE_FLAGS=-g3
 */

int main(int argc, char** argv, char** envp) {
	return EXIT_SUCCESS;
}

/*
 * When looking at the disassembly we see that the compiler moves the variable
 * from the stack into a register, does the add in the register and then returns
 * it to the stack
 */
int do_volatile(volatile int i) {
	i++;
	return i;
}

/*
 */
int do_regular(int i) {
	i++;
	return i;
}

/*
 * this one is the atomic case
 */
int do_atomic(int i) {
	__sync_add_and_fetch(&i, 1);
	return i;
}
