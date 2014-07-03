/*
 * This file is part of the linuxapi package.
 * Copyright (C) 2011-2014 Mark Veltzer <mark.veltzer@gmail.com>
 *
 * linuxapi is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * linuxapi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with linuxapi. If not, see <http://www.gnu.org/licenses/>.
 */

#include <firstinclude.h>
#include <stdio.h>	// for printf(3)
#include <stdlib.h>	// for EXIT_SUCCESS, atoi(3)
#include <unistd.h>	// for pause(2)

/*
 * This example shows the addresses of functions not yet resolved.
 */

int main(int argc, char** argv, char** envp) {
	void** p_printf=(void**)&printf;
	void** p_snprintf=(void**)&snprintf;
	void** p_atoi=(void**)&atoi;
	void* pp_printf=*p_printf;
	void* pp_snprintf=*p_snprintf;
	void* pp_atoi=*p_atoi;
	printf("&printf is %p\n", p_printf);
	printf("&snprintf is %p\n", p_snprintf);
	printf("&atoi is %p\n", p_atoi);
	printf("*&printf is %p\n", pp_printf);
	printf("*&snprintf is %p\n", pp_snprintf);
	printf("*&atoi is %p\n", pp_atoi);
	p_printf=(void**)&printf;
	p_snprintf=(void**)&snprintf;
	p_atoi=(void**)&atoi;
	pp_printf=*p_printf;
	pp_snprintf=*p_snprintf;
	pp_atoi=*p_atoi;
	printf("&printf is %p\n", p_printf);
	printf("&snprintf is %p\n", p_snprintf);
	printf("&atoi is %p\n", p_atoi);
	printf("*&printf is %p\n", pp_printf);
	printf("*&snprintf is %p\n", pp_snprintf);
	printf("*&atoi is %p\n", pp_atoi);
	while(true) {
		pause();
	}
	return EXIT_SUCCESS;
}
