/*
	This file is part of the linuxapi project.
	Copyright (C) 2011, 2012 Mark Veltzer <mark.veltzer@gmail.com>

	The linuxapi package is free software; you can redistribute it and/or
	modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 2.1 of the License, or (at your option) any later version.

	The linuxapi package is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public
	License along with the GNU C Library; if not, write to the Free
	Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
	02111-1307 USA.
*/

#include<us_helper.h> // for TRACE

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
