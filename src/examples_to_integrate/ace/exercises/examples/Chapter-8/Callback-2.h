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

#ifndef APG_CALLBACK2_H
#define APG_CALLBACK2_H

#include<firstinclude.h>
#include<ace/OS_NS_time.h>
#include<ace/streams.h>
#include<ace/Log_Msg_Callback.h>
#include<ace/Log_Record.h>
#include<ace/SString.h>
#include<ace/Time_Value.h>

class Callback:public ACE_Log_Msg_Callback {
	public:
		void log(ACE_Log_Record &log_record) {
			cerr << "Log Message Received:" << endl;
			unsigned long msg_severity = log_record.type ();
			ACE_Log_Priority prio = static_cast<ACE_Log_Priority> (msg_severity);
			const ACE_TCHAR *prio_name = ACE_Log_Record::priority_name (prio);
			cerr << "\tType: "
				<< ACE_TEXT_ALWAYS_CHAR (prio_name)
				<< endl;
			cerr << "\tLength: " << log_record.length () << endl;
			const time_t epoch = log_record.time_stamp ().sec ();
			cerr << "\tTime_Stamp: "
				<< ACE_TEXT_ALWAYS_CHAR (ACE_OS::ctime (&epoch))
				<< flush;
			cerr << "\tPid: " << log_record.pid () << endl;
			ACE_CString data (">> ");
			data += ACE_TEXT_ALWAYS_CHAR (log_record.msg_data ());
			cerr << "\tMsgData: " << data.c_str () << endl;
	}
};
#endif /* APG_CALLBACK2_H */
