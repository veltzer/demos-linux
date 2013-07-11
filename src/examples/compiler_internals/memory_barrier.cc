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
#include <stdlib.h>	// for EXIT_SUCCESS
#include <disassembly_utils.h>	// for disassemble_main()

/*
 * This example shows that a memory barrier does not produce any
 * machine instructions.
 *
 * this is for the source interleaving below...
 * EXTRA_COMPILE_FLAGS=-g3
 */

int main(int argc, char** argv, char** envp) {
	__asm ("" ::: "memory");
	__asm ("" ::: "memory");
	__asm ("" ::: "memory");
	__asm ("" ::: "memory");
	__asm ("" ::: "memory");
	__asm ("" ::: "memory");
	__asm ("" ::: "memory");
	__asm ("" ::: "memory");
	__asm ("" ::: "memory");
	__asm ("" ::: "memory");
	disassemble_me();
	return EXIT_SUCCESS;
}
