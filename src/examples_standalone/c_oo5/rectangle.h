#ifndef __rectangle_h
#define __rectangle_h

typedef struct _rectangle rectangle;

rectangle* create_rectangle(const int width, const int height);

#define METHOD_OF_TYPE(t) typedef t (*method_ ## t)(rectangle*, ...)
#define TYPE_OF_METHOD(t) method_ ## t
METHOD_OF_TYPE(int);
METHOD_OF_TYPE(void);

enum methods {
	m_get_width, // int
	m_get_height, // int
	m_set_width, // void
	m_set_height, // void
	m_area, // int
	m_destroy, // void
};

#define CALL_METHOD(o, m, t, args...) ((((TYPE_OF_METHOD(t)*)(*((void**)o))))[m])(o, ## args)

#endif // __rectangle_h
