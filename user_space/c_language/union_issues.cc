#include <stdio.h> // for printf(3)

/*
 *      This explorer issues with accessing structures which are in themselves
 *      inside unions...
 *
 *      This clearly shows that you can indeed access either fields within structures
 *      or fields which are not in structures...
 *
 *      It also explores how to get offsets of fields in structures...
 *
 *              Mark Veltzer
 *
 * EXTRA_LIBS=
 */

// this next macro calculates the offset of a field from the structure
// or union that contains it...
#define OffsetOf(StructName, FieldName) ((char *)(&(((StructName *)1)->FieldName)) - (char *)1)

typedef union _LARGE_INTEGER {
	struct {
		unsigned int LowPart;
		unsigned int HighPart;
	};
	long long QuadPart;
} LARGE_INTEGER;

int main(int argc, char **argv, char **envp) {
	// this is how you would use such a union...
	LARGE_INTEGER li;

	li.LowPart = 15;
	li.HighPart = 16;
	li.QuadPart = 17;
	// this is the offsets of it's relevant parts...
	printf("offset of LowPart is %d\n", OffsetOf(LARGE_INTEGER, LowPart));
	printf("offset of HighPart is %d\n", OffsetOf(LARGE_INTEGER, HighPart));
	printf("offset of QuadPart is %d\n", OffsetOf(LARGE_INTEGER, QuadPart));
	// or we could use the built in...
	printf("offset of LowPart is %d\n", __builtin_offsetof(LARGE_INTEGER, LowPart));
	printf("offset of HighPart is %d\n", __builtin_offsetof(LARGE_INTEGER, HighPart));
	printf("offset of QuadPart is %d\n", __builtin_offsetof(LARGE_INTEGER, QuadPart));
	return(0);
}
