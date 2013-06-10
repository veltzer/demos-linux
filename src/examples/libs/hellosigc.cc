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
#include <iostream>
#include <string>
#include <sigc++/signal_system.h>
#include <stdlib.h>	// for EXIT_SUCCESS

/*
 * This is a demo program for using the sigc++ signaling library
 *
 * EXTRA_COMPILE_FLAGS=-I/usr/include/sigc++-1.0 -I/usr/lib/sigc++-1.0/include
 * EXTRA_LINK_FLAGS=-lsigc -lpthread
 */

using namespace SigC;

void print(const std::string& str) {
	std::cout << str;
}

int main(int argc, char** argv, char** envp) {
	Signal1<void, const std::string&> printer;
	printer.connect(slot(print));
	printer("Hello, World!\n");
	return EXIT_SUCCESS;
}

// template void SigC::Signal1<void, basic_string<char, string_char_traits<char>, __default_alloc_template<true, 0> > const &, SigC::Marshal<void> >::emit(basic_string<char, string_char_traits<char>, __default_alloc_template<true, 0> > const &);
template void SigC::Signal1<void, std::basic_string<char, std::char_traits<char>, std::allocator<char> > const&, SigC::Marshal<void> >::emit(std::basic_string<char, std::char_traits<char>, std::allocator<char> > const&);

template SigC::SlotData *SigC::manage<SigC::SlotData>(SigC::SlotData *);

template char *std::basic_string<char, std::char_traits<char>, std::allocator<char> >::_S_construct<char const *>(char const *, char const *, std::allocator<char> const&, std::forward_iterator_tag);
