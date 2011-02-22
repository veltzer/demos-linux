#include "Foo.h"

/*
 * EXTRA_CMDS=pkg-config --cflags --libs ACE
 */

/*
 * This is the explicit instantiation file.
 * Even though it is compiled with flags that imply no
 * implicit instantiation, the code here explicitly causes
 * the templates to be compiled.
 */

template class Foo<int>;
