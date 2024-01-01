/*
 * This file is part of the demos-linux package.
 * Copyright (C) 2011-2024 Mark Veltzer <mark.veltzer@gmail.com>
 *
 * demos-linux is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * demos-linux is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with demos-linux. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __preprocessor_utils_h
#define __preprocessor_utils_h

/*
 * This creates a C preprocessor macro that returns the number
 * of arguments it gets.
 * References:
 * - https://stackoverflow.com/questions/2308243/macro-returning-the-number-of-arguments-it-is-given-in-c
 */

/* THIS IS A C FILE, NO C++ here */

#include <firstinclude.h>

/* The PP_NARG macro returns the number of arguments that have been
** passed to it.
*/

#define PP_NARG(...) PP_NARG_(__VA_ARGS__,PP_RSEQ_N())
#define PP_NARG_(...) PP_ARG_N(__VA_ARGS__)
#define PP_ARG_N(\
	_1, _2, _3, _4, _5, _6, _7, _8, _9,_10,\
	_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,\
	_21,_22,_23,_24,_25,_26,_27,_28,_29,_30,\
	_31,_32,_33,_34,_35,_36,_37,_38,_39,_40,\
	_41,_42,_43,_44,_45,_46,_47,_48,_49,_50,\
	_51,_52,_53,_54,_55,_56,_57,_58,_59,_60,\
	_61,_62,_63, N, ...) N
#define PP_RSEQ_N()\
	63,62,61,60,\
	59,58,57,56,55,54,53,52,51,50,\
	49,48,47,46,45,44,43,42,41,40,\
	39,38,37,36,35,34,33,32,31,30,\
	29,28,27,26,25,24,23,22,21,20,\
	19,18,17,16,15,14,13,12,11,10,\
	9, 8, 7, 6, 5, 4, 3, 2, 1, 0

#endif	/* !__preprocessor_utils_h */
