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

#include<firstinclude.h>
#include<ace/config-lite.h>
#include<ace/Synch.h>
#include<ace/Task.h>
#include<stdlib.h> // for EXIT_SUCCESS

/*
* EXTRA_CMDS=pkg-config --cflags --libs ACE
*/

class HA_Device_Repository {
	public:
		HA_Device_Repository() {
		}
		void update_device(int device_id) {
			mutex_.acquire();
			ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%t) Updating device %d\n"),device_id));
			ACE_OS::sleep(1);
			mutex_.release();
			// Let the other thread to aquire too
			ACE_OS::sleep(1);
		}
	private:
		ACE_Thread_Mutex mutex_;
};
class HA_CommandHandler:public ACE_Task_Base {
	public:
		enum {
			NUM_USES=10
		};
		HA_CommandHandler(HA_Device_Repository & rep):rep_(rep) {
		}
		virtual int svc(void) {
			ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%t) Handler Thread running\n")));
			for (int i=0; i < NUM_USES; i++) {
				this->rep_.update_device(i);
			}
			return(0);
		}
	private:
		HA_Device_Repository& rep_;
};

int ACE_TMAIN(int argc,ACE_TCHAR** argv,ACE_TCHAR** envp) {
	HA_Device_Repository rep;
	HA_CommandHandler handler1(rep);
	HA_CommandHandler handler2(rep);
	handler1.activate();
	handler2.activate();
	handler1.wait();
	handler2.wait();
	return EXIT_SUCCESS;
}
