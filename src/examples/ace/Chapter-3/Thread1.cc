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

#include <firstinclude.h>
#include <ace/Synch.h> // for the mutex
#include <ace/Thread.h> // for thread functions
#include <ace/Log_Msg.h> // for ACE_DEBUG
#include <stdlib.h> // for EXIT_SUCCESS, EXIT_FAILURE

/*
* EXTRA_COMPILE_CMDS=pkg-config --cflags ACE
* EXTRA_LINK_CMDS=pkg-config --libs ACE
*/

//Arguments that are to be passed to the worker thread are instances of this class.
class Args {
	public:
		Args(int iterations):mutex(),iterations(iterations) {
		}
		ACE_Thread_Mutex mutex;
		int iterations;
};

//The starting point for the worker threads
static void * worker(void *arguments) {
	Args *arg=(Args*)arguments;
	for(int i=0;i<arg->iterations;i++) {
		ACE_DEBUG((LM_DEBUG,"(%t) Trying to get a hold of this iteration\n"));
		// lock the mutex
		arg->mutex.acquire();
		ACE_DEBUG((LM_DEBUG, "(%t) This is iteration number %d\n", i));
		// simulate work
		ACE_OS::sleep(2);
		// release the mutex
		arg->mutex.release();
	}
	return(0);
}

int ACE_TMAIN(int argc,ACE_TCHAR** argv,ACE_TCHAR** envp) {
	if(argc<2) {
		ACE_OS::printf("Usage: %s <number_of_threads> <number_of_iterations>\n",argv[0]);
		ACE_OS::exit(EXIT_FAILURE);
	}
	//Setup the arguments
	Args arg(ACE_OS::atoi(argv[2]));
	unsigned int thread_num=ACE_OS::atoi(argv[1]);

	// create the data structures needed
	ACE_thread_t* threads=new ACE_thread_t[thread_num];
	ACE_hthread_t* threadHandles=new ACE_hthread_t[thread_num];

	//Spawn the worker threads
	if(ACE_Thread::spawn_n(
		threads,//id's for each of the threads
		thread_num,//number of threads to spawn
		worker,//entry point for new thread
		(void*)&arg,//args to worker
		THR_JOINABLE | THR_NEW_LWP,//flags
		ACE_DEFAULT_THREAD_PRIORITY,
		NULL,
		0,
		threadHandles)!=thread_num) {
			ACE_DEBUG((LM_DEBUG, "Error in spawning thread\n"));
	}
	//Wait for all the threads to exit before you let the main fall through
	//and have the process exit.
	for(unsigned int i=0;i<thread_num;i++) {
		ACE_Thread::join(threads[i]);
	}
	return EXIT_SUCCESS;
}
