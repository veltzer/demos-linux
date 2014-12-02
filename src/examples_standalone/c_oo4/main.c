#include "rectangle.h"
#include <stdio.h>
#include <stdlib.h>
#include <alloca.h>

int main() {
	rectangle* r=create_rectangle(5,6);
	printf("r is at %p\n", r);
	printf("the area of the rectangle is %d\n", rectangle_area(r));
	rectangle_destroy(r);

	rectangle* r2=(rectangle*)alloca(sizeof_rectangle());
	construct_rectangle(r2, 7, 8);
	printf("r2 is at %p\n", r2);
	printf("the area of the rectangle is %d\n", rectangle_area(r2));
	return 0;
}
