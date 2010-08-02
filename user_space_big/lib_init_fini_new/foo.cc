#include <stdio.h> // for printf(3)

#include "us_helper.hh"

void foo_init(void) {
	TRACE("start");
}
void foo_init(void) __attribute__((constructor));
void foo_init105(void) {
	TRACE("start");
}
void foo_init105(void) __attribute__((constructor(105)));
void foo_init104(void) {
	TRACE("start");
}
void foo_init104(void) __attribute__((constructor(104)));
void foo_init103(void) {
	TRACE("start");
}
void foo_init103(void) __attribute__((constructor(103)));

void foo_fini(void) {
	TRACE("start");
}
void foo_fini(void) __attribute__((destructor));
void foo_fini105(void) {
	TRACE("start");
}
void foo_fini105(void) __attribute__((destructor(105)));
void foo_fini104(void) {
	TRACE("start");
}
void foo_fini104(void) __attribute__((destructor(104)));
void foo_fini103(void) {
	TRACE("start");
}
void foo_fini103(void) __attribute__((destructor(103)));
