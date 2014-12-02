#ifndef __rectangle_h
#define __rectangle_h

typedef struct _rectangle rectangle;

rectangle* create_rectangle(const int width, const int height);
int sizeof_rectangle();
void construct_rectangle(rectangle*, const int width, const int height);
void rectangle_destroy(rectangle*);
int rectangle_get_width(const rectangle const * r);
int rectangle_get_height(const rectangle const * r);
void rectangle_set_width(rectangle * r, const int width);
void rectangle_set_height(rectangle * r, const int height);
int rectangle_area(const rectangle const * r);

#endif // __rectangle_h
