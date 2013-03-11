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
#include <stdio.h>	// for printf(3)
#include <stdlib.h>	// for EXIT_SUCCESS
#include <us_helper.h>	// for TRACE(), my_system()

/*
 * This is a demo to show how sync_synchronize() is implemented...
 * You should see the __sync_synchronize which is a machine memory barrier
 * translated to a "lock orl" instruction in assembly which is an instruction
 * to the core you are running on to stop reordering writes and reads
 * and drop all cache assumptions.
 * So you should see one instruction in the disassembly of this code.
 * And indeed this is what we see.
 *
 * Take note that this is a also a compiler barrier. It prevents the compiler
 * from reordering around this code. You don't see any assembly emitted for
 * that since a compiler barrier is only a compile time instruction about
 * code emittion.
 *
 * EXTRA_LINK_FLAGS=-lpthread
 */

int main(int argc, char** argv, char** envp) {
	TRACE("start");
	__sync_synchronize();
	__sync_synchronize();
	__sync_synchronize();
	__sync_synchronize();
	__sync_synchronize();
	TRACE("end");
	my_system("objdump --disassemble --source %s", argv[0]);
	return EXIT_SUCCESS;
}
