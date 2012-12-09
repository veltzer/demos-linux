/*
	This file is part of the linuxapi project.
	Copyright (C) 2011, 2012 Mark Veltzer <mark.veltzer@gmail.com>

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
#include <iostream>
#include <string>
#include <sigc++/signal_system.h>
#include <stdlib.h> // for EXIT_SUCCESS

/*
* This is a demo program for using the sigc++ signaling library
*
* EXTRA_LIBS=-I/usr/include/sigc++-1.0 -I/usr/lib/sigc++-1.0/include -lsigc -lpthread
*/

using namespace SigC;

void print(const std::string& str) {
	std::cout << str;
}

int main(int argc,char** argv,char** envp) {
	Signal1<void, const std::string&> printer;
	printer.connect(slot(print));
	printer("Hello, World!\n");
	return EXIT_SUCCESS;
}

//template void SigC::Signal1<void, basic_string<char, string_char_traits<char>, __default_alloc_template<true, 0> > const &, SigC::Marshal<void> >::emit(basic_string<char, string_char_traits<char>, __default_alloc_template<true, 0> > const &);
template void SigC::Signal1<void, std::basic_string<char, std::char_traits<char>, std::allocator<char> > const&, SigC::Marshal<void> >::emit(std::basic_string<char, std::char_traits<char>, std::allocator<char> > const&);

template SigC::SlotData *SigC::manage<SigC::SlotData>(SigC::SlotData *);

template char *std::basic_string<char, std::char_traits<char>, std::allocator<char> >::_S_construct<char const *>(char const *, char const *, std::allocator<char> const&, std::forward_iterator_tag);
