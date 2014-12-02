#include "rectangle.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
	rectangle* r=create_rectangle(5,6);
	printf("the area of the rectangle is %d\n", rectangle_area(r));
	rectangle_destroy(r);
	return 0;
}
