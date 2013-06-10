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
