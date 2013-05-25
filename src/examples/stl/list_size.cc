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
#include <iostream>	// for std::cout, std::endl
#include <list>	// for std::list<T>, std::list<T>::iterator
#include <stdlib.h>	// for EXIT_SUCCESS
#include <measure.h>	// for measure_start(), measure_end(), measuer_print()

/*
 * Example showing that list.size() in STL is O(N)!
 *
 * Notes:
 * - there is a claim by one of the participants in the blog that in C++11
 * list.size() is const time. This example is compiled with C++11 support
 * and clearly shows that this is not the case.
 *
 * References:
 * http://www.linuxprogrammingblog.com/std-list-size-complexity
 *
 * EXTRA_COMPILE_FLAGS=-std=c++11
 */

int main(int argc, char** argv, char** envp) {
	std::list<int> l;
	const unsigned int size_of_list=1000000;
	for (unsigned int i = 0; i < size_of_list; i++)
		l.push_back (i);
	std::cout << "size of list is " << size_of_list << std::endl;
	measure m;
	measure_start(&m, "one l.size()");
	volatile __attribute__((unused)) int s;
	const unsigned int attempts=1000;
	for (unsigned int i = 0; i < attempts; i++) {
		s=l.size();
	}
	measure_end(&m, "one l.size()");
	measure_print(&m, "one l.size()", attempts);
	return EXIT_SUCCESS;
}
