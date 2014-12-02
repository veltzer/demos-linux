#include "rectangle.h"

int rectangle_get_width(const rectangle const * r) {
	return r->width;
}

int rectangle_get_height(const rectangle const * r) {
	return r->height;
}

void rectangle_set_width(rectangle * r, const int width) {
	r->width=width;
}

void rectangle_set_height(rectangle * r, const int height) {
	r->height=height;
}

int rectangle_area(const rectangle const * r) {
	return r->height*r->width;
}
