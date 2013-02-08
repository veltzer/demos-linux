/*
	This file is part of the linuxapi project.
	Copyright (C) 2011-2013 Mark Veltzer <mark.veltzer@gmail.com>

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
#include <iostream> // for std::cout, std::endl
#include <list> // for std::list<T>, std::list<T>::iterator
#include <vector> // for std::list<T>, std::list<T>::iterator
#include <stdlib.h> // for EXIT_SUCCESS, EXIT_FAILURE;

int main(int argc,char** argv,char** envp) {
	if(argc!=4) {
		std::cerr << argv[0] << ": usage " << argv[0] << " [size] [postoact] [postoremove]" << std::endl;
		return EXIT_FAILURE;
	}
	int size=atoi(argv[1]);
	int postoact=atoi(argv[2]);
	int postoremove=atoi(argv[3]);
	//std::list<int> l;
	std::vector<int> l;
	for(int i=0;i<size;i++) {
		l.push_back(i);
	}

	// remove an element in a position already passed
	//std::list<int>::iterator i;
	std::vector<int>::iterator i;
	int counter=0;
	for(i=l.begin();i!=l.end();i++) {
		if(counter==postoact) {
			//l.remove(postoremove);
			l.erase(l.begin()+postoremove);
		}
		std::cout << "visiting " << *i << std::endl;
		counter++;
	}
	return EXIT_SUCCESS;
}
