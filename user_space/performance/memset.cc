#include <string.h> // for memset
#include <stdlib.h> // for malloc
#include <wchar.h> // for wmemset

/*
 * This example explores the workings of memset...
 *
 * From the disassembly we can see that:
 * - no call to memset is used since memset is a macro.
 *	(you cannot break on memset in the debugger and actually stop on that function)
 * - this means that memset inline assembly is inserted into our code.
 * - the compiler uses "rep stos" to do the actual memsetting which is
 *	a single machine instruction on Intel which means that it is very efficient.
 * - the wmemset function is not inline assembly (you can break on it in the debugger)
 * - this example does not explore the inner workins of the wmemset function (meanins whether
 *	it is implemented efficiently or not).
 *
 * TODO: find out how wmemset work and discuss it's efficiency.
 *
 *		Mark Veltzer
 *
 * EXTRA_LIBS=
 */
int main(int argc, char **argv, char **envp) {
	const int size = 1000;
	char      *buf = (char *)malloc(size);

	memset(buf, 0, size);
	for (int i = 0; i < size; i++) {
		buf[i] = 0;
	}
	wchar_t *buf2 = (wchar_t *)malloc(size * sizeof(wchar_t));
	wmemset(buf2, 0, size);
	return(0);
}
