/*
	This file is part of the linuxapi project.
	Copyright (C) 2011, 2012 Mark Veltzer <mark.veltzer@gmail.com>

	The linuxapi package is free software; you can redistribute it and/or
	modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 2.1 of the License, or (at your option) any later version.

	The linuxapi package is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public
	License along with the GNU C Library; if not, write to the Free
	Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
	02111-1307 USA.
*/

#include <firstinclude.h>
#include <us_helper.h> // for my_system()

/*
* This example shows that a real compiler barrier does not emit any machine instructions.
* It simply uses the compiler barrier a lot and shows the disassembly...
*/

int main(int argc,char** argv,char** envp) {
	asm volatile("":::"memory");
	asm volatile("":::"memory");
	asm volatile("":::"memory");
	asm volatile("":::"memory");
	asm volatile("":::"memory");
	asm volatile("":::"memory");
	asm volatile("":::"memory");
	asm volatile("":::"memory");
	asm volatile("":::"memory");
	asm volatile("":::"memory");
	asm volatile("":::"memory");
	asm volatile("":::"memory");
	asm volatile("":::"memory");
	asm volatile("":::"memory");
	asm volatile("":::"memory");
	asm volatile("":::"memory");
	asm volatile("":::"memory");
	asm volatile("":::"memory");
	asm volatile("":::"memory");
	my_system("objdump --disassemble --source %s",argv[0]);
	return EXIT_SUCCESS;
}
