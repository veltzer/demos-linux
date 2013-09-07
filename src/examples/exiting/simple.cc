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
#include <unistd.h>	// for _exit(2)
#include <stdlib.h>	// for EXIT_SUCCESS

/*
 * This example just calls the glibc wrapper for _exit(2). If you
 * strace this one then you will see that glibc actually calls
 * the exit_group(2) system call and not _exit(2) since the real
 * _exit(2) (the one provided by the kernel, not the wrapper)
 * will terminate just the calling thread and not other threads.
 *
 * Note that you can echo $? after this program is done to see
 * the exit code.
 *
 * TODO:
 * - make this program strace(1) itself.
 */

int main(int argc, char** argv, char** envp) {
	_exit(5);
	return EXIT_SUCCESS;
}
