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

int main(int argc, char** argv, char** envp) {
	std::list<int> l;
	l.push_back(0);	// Insert a new element at the end
	l.push_front(0);// Insert a new element at the beginning
	l.insert(++l.begin(), 2);	// Insert "2" before position of first argument
	// (Place before second argument)
	l.push_back(5);
	l.push_back(6);

	// lets iterate and print
	std::list<int>::iterator i;
	for(i=l.begin(); i!=l.end(); i++)
		std::cout << *i << " ";
	std::cout << std::endl;
	return EXIT_SUCCESS;
}
