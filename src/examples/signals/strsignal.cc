/*
 * This file is part of the linuxapi project.
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
#include <string.h>	// for strsignal(3)
#include <stdio.h>	// for printf(3)
#include <signal.h>	// for signal names
#include <us_signals.h>	// for print_signal_names()

/*
 * This is an example of using strsignal(3) to print out signal names.
 * See the us_signals.h header file for more details.
 */

int main(int argc, char** argv, char** envp) {
	print_signal_names();
	return EXIT_SUCCESS;
}
