#include "rectangle.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
	rectangle* r=(rectangle*)malloc(sizeof(rectangle));
	rectangle_set_width(r,5);
	rectangle_set_height(r,6);
	printf("the area of the rectangle is %d\n", rectangle_area(r));
	return 0;
}
