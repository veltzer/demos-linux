#include <stdio.h> // for printf(3)

#include "us_helper.hh"

void bar_init(void) {
	TRACE("start");
}
void bar_init(void) __attribute__((constructor));
void bar_init105(void) {
	TRACE("start");
}
void bar_init105(void) __attribute__((constructor(105)));
void bar_init104(void) {
	TRACE("start");
}
void bar_init104(void) __attribute__((constructor(104)));
void bar_init103(void) {
	TRACE("start");
}
void bar_init103(void) __attribute__((constructor(103)));

void bar_fini(void) {
	TRACE("start");
}
void bar_fini(void) __attribute__((destructor));
void bar_fini105(void) {
	TRACE("start");
}
void bar_fini105(void) __attribute__((destructor(105)));
void bar_fini104(void) {
	TRACE("start");
}
void bar_fini104(void) __attribute__((destructor(104)));
void bar_fini103(void) {
	TRACE("start");
}
void bar_fini103(void) __attribute__((destructor(103)));
