/*
 * This file is part of the linuxapi project.
 * Copyright (C) 2011-2013 Mark Veltzer <mark.veltzer@gmail.com>
 *
 * The linuxapi package is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * The linuxapi package is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with the GNU C Library; if not, write to the Free
 * Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 * 02111-1307 USA.
 */

#include <firstinclude.h>
#include <stdio.h>	// for printf(3)
#include <stdlib.h>	// for EXIT_SUCCESS
#include <sys/capability.h> // for cap_get_proc(3), cap_set_flag(3)
#include <us_helper.h>	// for CHECK_NOT_M1()

/*
 * This is an example showing setting of capabilities
 *
 * EXTRA_LINK_FLAGS=-lcap
 */

int main(int argc, char** argv, char** envp) {
	CHECK_ASSERT(CAP_IS_SUPPORTED(CAP_NET_BIND_SERVICE));
	cap_t caps=(cap_t)CHECK_NOT_NULL(cap_get_proc());
	cap_value_t cap_list[1];
	cap_list[0] = CAP_NET_BIND_SERVICE;
	CHECK_NOT_M1(cap_set_flag(caps, CAP_EFFECTIVE, 1, cap_list, CAP_SET));
	CHECK_NOT_M1(cap_set_proc(caps));
	CHECK_NOT_M1(cap_free(caps));
	CHECK_NOT_M1(setuid(1000));
	return EXIT_SUCCESS;
}
