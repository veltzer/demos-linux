#include <bfd.h>
#include <iostream>
#include <stdlib.h>

/*
 * This program demos the libbfd library which enabled you
 * to scan and manipulate object files in various formats
 *
 *			Mark Veltzer
 *
 * EXTRA_LIBS=-lbfd
 */
int main(int argc, char** argv, char** envp) {
	bfd_init();
	bfd *b = bfd_openr("/bin/ls", NULL);
	if (!b) {
		std::cerr << "problem with open\n";
		return(1);
	}
	bfd_format format = bfd_object;
	bool ok_format = bfd_check_format(b, format);
	if (!ok_format) {
		std::cerr << "problem with bfd_check_format\n";
		return(1);
	}
	const char *name = bfd_format_string(format);
	std::cout << "format is " << name << "\n";
	bool res = bfd_close(b);
	if (!res) {
		std::cerr << "problem with close\n";
		return(1);
	}
	return EXIT_SUCCESS;
}
