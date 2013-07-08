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
#include <unistd.h>	// for sysconf(3), _SC_NPROCESSORS_ONLN
#include <us_helper.h>	// for TRACE()
#include <cpu_set_utils.h>	// for print_cpu_set()

/*
 * This is a demo of how to use a cpu set
 */
int main(int argc, char** argv, char** envp) {
	TRACE("number of cpus is %ld", sysconf(_SC_NPROCESSORS_ONLN));
	cpu_set_t cpuset;
	CPU_ZERO(&cpuset);
	for(int i=0; i<500; i+=100) {
		CPU_SET(i, &cpuset);
	}
	print_cpu_set(&cpuset);
	return EXIT_SUCCESS;
}
