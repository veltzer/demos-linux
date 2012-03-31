#include<cxxabi.h>
#include<string.h>

/*
 * function to demangle name using the C++ API for doing so
 * There is no C API for demangling since C names do not get
 * mangled.
 */

inline void error_demangle(
	char* symbol,
	char* result_name,
	unsigned int max_name,
	char* result_offset,
	unsigned int max_offset
) {
	char *begin_name = NULL, *begin_offset = NULL, *end_offset = NULL;

	// find parentheses and +address offset surrounding the mangled name:
	// ./module(function+0x15c) [0x8048a6d]
	for(char *p = symbol; *p; ++p)
	{
		if (*p == '(')
			begin_name = p;
		else if (*p == '+')
			begin_offset = p;
		else if (*p == ')' && begin_offset) {
			end_offset = p;
			break;
		}
	}
	if (begin_name && begin_offset && end_offset && begin_name < begin_offset)
	{
		*begin_name++ = '\0';
		*begin_offset++ = '\0';
		*end_offset = '\0';
		// mangled name is now in [begin_name, begin_offset) and caller
		// offset in [begin_offset, end_offset). now apply
		// __cxa_demangle():
		// allocate string which will be filled with the demangled function name
		size_t funcnamesize = 256;
		char* funcname = (char*)malloc(funcnamesize);
		int status;
		char* ret = abi::__cxa_demangle(begin_name,funcname,&funcnamesize, &status);
		if (status == 0) {
			funcname = ret; // use possibly realloc()-ed string
			strncpy(result_name,ret,max_name);
			strncpy(result_offset,begin_offset,max_offset);
		}
		else {
			// demangling failed. Output function name as a C function with
			// no arguments.
			snprintf(result_name,max_name,"%s()",begin_name);
			snprintf(result_offset,max_offset,"%s",begin_offset);
			//fprintf(stderr, "error demangle: %s : %s()+%s\n", symbol, begin_name, begin_offset);
		}
	}
	else
	{
		// couldn't parse the line? print the whole line.
		fprintf(stderr, "error parsing: %s\n", symbol);
	}
}
