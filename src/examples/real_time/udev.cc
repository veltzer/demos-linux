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

#include<stdlib.h> // for EXIT_SUCCESS

/*
 * This application should show how to listen to udev events using API...
 *
 * Why would you want to do this ?
 * - you want a real small RT system and don't want udev on it.
 * - you hate the "system" approach that udev takes to running rules.
 * - you want to listen to udev events in parallel to other things with tight integration.
 * - you want to reduce boot time and want to do things in parallel
 *	(listen to drivers loading while doing other initialization work).
 *
 * Strictly speaking this demo is not a real time demo per se. It is a demo that will
 * usually be used on embedded systems where a full udev is not available but not
 * necessarily a real time system.
 */

int main(int argc,char** argv,char** envp) {
	return EXIT_SUCCESS;
}
