#include"Foo.h"

/*
 * This code uses Foo.h but will not be compiled with
 * the standard flags but rather without implicit instantiation.
 */
void bar() {
	Foo<int> f;
	f.setT(5);
}
