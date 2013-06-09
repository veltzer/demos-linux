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

/*
 * An example of how to write a classic "Hello, World!\n" program in assembly
 */

#include <asm/unistd.h>
#include <syscall.h>

.section	.rodata
hello:
	.string	"Hello, World!\n"
.text
.global	main
.type main, @function
main:
	## write(1,hello,14)
	movl $SYS_write, %eax
	movl $1, %ebx
	movl $hello, %ecx
	movl $14, %edx
	int $0x80
	
	## _exit(7)
	movl $7, %ebx
	movl $SYS_exit, %eax
	int $0x80
