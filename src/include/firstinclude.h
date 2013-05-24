/*
 *      This file is part of the linuxapi project.
 *      Copyright (C) 2011-2013 Mark Veltzer <mark.veltzer@gmail.com>
 *
 *      The linuxapi package is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU Lesser General Public
 *      License as published by the Free Software Foundation; either
 *      version 2.1 of the License, or (at your option) any later version.
 *
 *      The linuxapi package is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 *      Lesser General Public License for more details.
 *
 *      You should have received a copy of the GNU Lesser General Public
 *      License along with the GNU C Library; if not, write to the Free
 *      Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 *      02111-1307 USA.
 */

#ifndef __firstinclude_h
#define __firstinclude_h

/* THIS IS C FILE, NO C++ here */

/*
 * This is the first file you should include from user space apps
 */

// needed for SCHED_IDLE, SCHED_BATCH, sched_getcpu() and more
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif	// _GNU_SOURCE

// Needed to get REG_EIP from ucontext.h and more
#ifndef __USE_GNU
#define __USE_GNU
#endif	// __USE_GNU

#endif	/* !__firstinclude_h */
