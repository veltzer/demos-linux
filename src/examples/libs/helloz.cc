#include <zlib.h> // for compress2(3)
#include <stdio.h> // for printf(3)
#include <string.h> // for strlen(3)

/*
 * A simple example of using the libz library.
 *
 * EXTRA_LIBS=-lz
 *
 *	Mark Veltzer
 *
 */
int main(int argc, char** argv, char** envp) {
	const char *source = "Merry had a little lamb";
	unsigned long sourcelen = strlen(source);
	unsigned long destlen = 100;
	unsigned char *dest = new unsigned char[destlen];
	int code = compress2(dest, &destlen, (const unsigned char *)source, sourcelen, Z_DEFAULT_COMPRESSION);
	if (code != Z_OK) {
		printf("compress failed with code [%d]\n", code);
		return(1);
	}
	printf("result is [%s]\n", dest);
	return(0);
}
