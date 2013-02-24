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
#include <sys/types.h>	// for ftok(3)
#include <sys/ipc.h>	// for ftok(3)
#include <stdio.h>	// for printf(3)
#include <stdlib.h>	// for EXIT_SUCCESS

/*
 * This example shows that you actually need to point to a real existing file
 * name in order for ftok to succeed...
 */

int main(int argc, char** argv, char** envp) {
	key_t k1=ftok("/tmp/no_such_file", 17);
	printf("key with no file is %d\n", k1);
	key_t k2=ftok("/etc/passwd", 17);
	printf("key with file is %d\n", k2);
	return EXIT_SUCCESS;
}
