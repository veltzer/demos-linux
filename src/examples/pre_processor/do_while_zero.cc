/*
 * This file is part of the linuxapi package.
 * Copyright (C) 2011-2013 Mark Veltzer <mark.veltzer@gmail.com>
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
#include <stdlib.h>	// for EXIT_SUCCESS
#include <us_helper.h>	// for __stringify()

/*
 * This example explains the command do { ... } while(0) macro construct.
 *
 * References:
 * http://stackoverflow.com/questions/1067226/c-multi-line-macro-do-while0-vs-scope-block
 */

#define wrong_macro() printf("part1\n"); printf("part2\n")
#define right_macro() do { printf("part1\n"); printf("part2\n"); } while(0)

int main(int argc, char** argv, char** envp) {
	// this should not print anything and yet it does...
	if(false)
		wrong_macro();
	if(false)
		right_macro();
	return EXIT_SUCCESS;
}
