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
#include <ace/Log_Msg.h>// for ACE_DEBUG and ACE_ERROR

/*
 * An example to show boost smart pointers.
 *
 * Notes:
 * - notice that the destructor of A gets called even thought there
 * is no explicit 'delete' in the code (that is the whole point of
 * smart pointers...)
 * - notice that we did not have to link with any boost libraries:
 * the smart_ptr is all in the header files.
 *
 * References:
 * http://www.codeproject.com/Articles/8394/Smart-Pointers-to-boost-your-code#The%20first:%20boost::scoped_ptrT
 *
 */

int main(int argc, char** argv, char** envp) {
	return EXIT_SUCCESS;
}
