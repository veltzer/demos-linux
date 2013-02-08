/*
	This file is part of the linuxapi project.
	Copyright (C) 2011-2013 Mark Veltzer <mark.veltzer@gmail.com>

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

#ifndef __kernel_helper_h
#define __kernel_helper_h

// do we want debug information ?!?
//#define DO_DEBUG
// TODO - the macros here could (and should?!?) be turned into inline functions...

#ifdef DO_DEBUG
#define PR_DEBUG(fmt, args...) printk(KERN_DEBUG "DEBUG %s %s %s %d: " fmt "\n", THIS_MODULE->name, __BASE_FILE__, __FUNCTION__, __LINE__, ## args)
#else
#define PR_DEBUG(fmt, args...)
#endif
// errors are always shown
#define PR_ERROR(fmt, args...) printk(KERN_ERR "ERROR %s %s %s %d: " fmt "\n", THIS_MODULE->name, __BASE_FILE__, __FUNCTION__, __LINE__, ## args)

// prints are always shown
#define PR_INFO(fmt, args...) printk(KERN_INFO "INFO %s %s %s %d: " fmt "\n", THIS_MODULE->name, __BASE_FILE__, __FUNCTION__, __LINE__, ## args)

static inline int memcheck(void *buf, char val, unsigned int size) {
	unsigned int i;
	char* cbuf=(char *)buf;
	for (i=0; i < size; i++) {
		if(cbuf[i]!=val) {
			PR_ERROR("value at %u is %c and not %c", i, cbuf[i], val);
			return(-EFAULT);
		}
	}
	return(0);
}

#endif // __kernel_helper_h
