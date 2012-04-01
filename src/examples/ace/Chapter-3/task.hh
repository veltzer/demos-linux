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

#ifndef TASK_H
#define TASK_H

#include<ace/Task.h>
#include<ace/Barrier.h>

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/*
 * This is our basic thread-pool Task. We have a choice of pool size
 * on the open() and the usual svc() and close() methods.
 *
 * A new addition is the ACE_Barrier object. This will allow the
 * synchronization of our svc() methods so that they all start at the
 * "same" time. The normal case may allow one thread to start working
 * earlier than others. There's no real harm in it but you can get
 * better "work by thread" statistics if they start out together.
 */
class Task : public ACE_Task<ACE_MT_SYNCH> {
public:

	typedef ACE_Task<ACE_MT_SYNCH> inherited;

	Task(void);
	~Task(void);

	/*
	 * I really wanted this to be called open() but that was already
	 * claimed by the Task framework. start() will kick off our thread
	 * pool for us.
	 */
	int start(int threads = 1);

	virtual int svc(void);

	virtual int close(u_long flags = 0);

protected:
	ACE_Barrier *barrier_;
};

#endif
