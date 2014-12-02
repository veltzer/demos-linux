#include "rectangle.h"
#include <stdio.h>
#include <stdlib.h>
#include <alloca.h>

int main() {
	rectangle* r=create_rectangle(5,6);
	CALL_METHOD(r, m_set_width, void, 8);
	printf("width is %d\n", CALL_METHOD(r, m_get_width, int));
	printf("the area of the rectangle is %d\n", CALL_METHOD(r, m_area, int));
	CALL_METHOD(r, m_destroy, void);
	return 0;
}
