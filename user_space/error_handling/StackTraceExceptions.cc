#include <iostream>
#include <stdlib.h>
#include <execinfo.h>
#include <exception>
#include "demangle.hh"

class TracedException : public std::exception {
private:
	// how many frames to drop from the stack frame
	// setting this to 0 will show you functions above 'main'
	// that you are probably not interested in
	static const int drop_frames = 2;
	static const int size = 25;
	void             *array[size];
	int nSize;
	char             **symbols;
public:
	TracedException() {
		nSize = backtrace(array, size);
		symbols = backtrace_symbols(array, nSize);
	}


	// TODO: need to C++'ize the next two methods
	void release() {
		free(symbols);
	}


	void print() {
		for (int i = nSize - drop_frames - 1; i >= 0; i--) {
			char *symbol = symbols[i];
			char result_name[256];
			char result_offset[256];
			error_demangle(symbol, result_name, 256, result_offset, 256);
			std::cerr
			        << symbol << ","
			        << result_name << ","
			        << result_offset
			        << std::endl;
		}
	}
};

void low_func() {
	throw TracedException();
}


void mid_func() {
	low_func();
}


void top_func() {
	mid_func();
}


int main(int argc, char **argv, char **envp) {
	try {
		top_func();
	} catch (TracedException e) {
		e.print();
	}
	return(0);
}
