/* This is a demo program for using the sigc++ signaling library
 *
 * EXTRA_LIBS=-I/usr/include/sigc++-1.0 -I/usr/lib/sigc++-1.0/include -lsigc -lpthread
 * */

#include <iostream>
#include <string>
#include <sigc++/signal_system.h>

using namespace SigC;

void print(const std::string& str) {
	std::cout << str;
}


int main(int args, char **argv) {
	Signal1<void, const std::string&> printer;
	printer.connect(slot(print));
	printer("Hello, World!\n");
	return(0);
}


//template void SigC::Signal1<void, basic_string<char, string_char_traits<char>, __default_alloc_template<true, 0> > const &, SigC::Marshal<void> >::emit(basic_string<char, string_char_traits<char>, __default_alloc_template<true, 0> > const &);
template void SigC::Signal1<void, std::basic_string<char, std::char_traits<char>, std::allocator<char> > const&, SigC::Marshal<void> >::emit(std::basic_string<char, std::char_traits<char>, std::allocator<char> > const&);

template SigC::SlotData *SigC::manage<SigC::SlotData>(SigC::SlotData *);

template char *std::basic_string<char, std::char_traits<char>, std::allocator<char> >::_S_construct<char const *>(char const *, char const *, std::allocator<char> const&, std::forward_iterator_tag);
