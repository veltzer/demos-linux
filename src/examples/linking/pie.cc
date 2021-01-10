/*
 * This file is part of the linuxapi package.
 * Copyright (C) 2011-2021 Mark Veltzer <mark.veltzer@gmail.com>
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
#include <proc_utils.h>	// for proc_print_mmap_self()

/*
 * This example shows how to compile an executable with position
 * independent code.
 * You can see that it is position independent by running this one again
 * and again and seeing the different addresses from the kernel ASLR.
 *
 * What you need to do?
 * - compile object with -fpie/-fPIE.
 * - link with -pie.
 *
 * Notes:
 * - -fpie/-fPIE make your code run a little slower on some platformas that do
 * not have elegant relative jump instructions (like i386 but not ia64).
 * - The PIE feature combined with ASLR (Address Space Layout Randomization) which
 * has been a feature of the Linux kernel for some time now, will give you better
 * protection against hackers.
 *
 * EXTRA_COMPILE_FLAGS=-fPIE
 * EXTRA_LINK_FLAGS=-pie
 */

int main(int argc, char** argv, char** envp) {
	proc_print_mmap_self();
	return EXIT_SUCCESS;
}
