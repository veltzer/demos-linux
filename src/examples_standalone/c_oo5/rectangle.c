#include "rectangle.h"
#include <stdlib.h>
#include <stdio.h>

typedef void (*method)(rectangle*, ...);

static method vtable[];

typedef struct _rectangle {
	method* vtable;
	int width;
	int height;
} rectangle;

rectangle* create_rectangle(const int height, const int width) {
	rectangle* this=(rectangle*)malloc(sizeof(rectangle));
	this->vtable=vtable;
	this->height=height;
	this->width=width;
	return this;
}

static void destroy(rectangle* r) {
	free(r);
}

static int get_width(const rectangle const * r) {
	return r->width;
}

static int get_height(const rectangle const * r) {
	return r->height;
}

static void set_width(rectangle * r, const int width) {
	r->width=width;
}

static void set_height(rectangle * r, const int height) {
	r->height=height;
}

static int area(const rectangle const * r) {
	return r->height*r->width;
}

static method vtable[]={
	(method)get_width,
	(method)get_height,
	(method)set_width,
	(method)set_height,
	(method)area,
	(method)destroy,
};
