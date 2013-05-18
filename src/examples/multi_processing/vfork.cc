/*
 * This file is part of the linuxapi project.
 * Copyright (C) 2011-2013 Mark Veltzer <mark.veltzer@gmail.com>
 *
 * The linuxapi package is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * The linuxapi package is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with the GNU C Library; if not, write to the Free
 * Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 * 02111-1307 USA.
 */

#include <firstinclude.h>
#include <unistd.h>	// for fork(2)
#include <stdio.h>	// for fgets(3)
#include <sys/types.h>	// for waitid(2)
#include <sys/wait.h>	// for waitid(2)
#include <stdlib.h>	// for exit(3), atoi(3), EXIT_SUCCESS, EXIT_FAILURE
#include <string.h>	// for strsignal(3)
#include <sys/types.h>	// for vfork(2)
#include <unistd.h>	// for vfork(2)
#include <us_helper.h>	// for CHECK_NOT_M1(), CHECK_ZERO()
#include <multi_processing.h>	// for print_code(), print_status()

/*
 * An example of using vfork(2)
 *
 * Notes:
 * - if you let the child return from main it will cause a problem.
 * - tracing seems to work fine and sleep(3) also.
 * - writing to the parent data does not seem to be harmful too.
 * - result: the return from the function in which vfork was called is the one
 * causing the segfault.
 * - the segfault seems to be in the parent not in the child. Need to check this
 * for sure.
 * - the results of using vfork incorrectly (not for exec) are indeed undefined as the manual page says.
 */
int global_data=42;

int main(int argc, char** argv, char** envp) {
	TRACE("this is the parent");
	pid_t child_pid=CHECK_NOT_M1(vfork());
	if (child_pid==0) {
		TRACE("this is the child");
		global_data++;
		// while(true) {
		// }
		CHECK_ZERO(sleep(10));
	} else {
		TRACE("this is the parent");
		bool over=false;
		while(!over) {
			TRACE("waiting for the child...");
			siginfo_t info;
			CHECK_NOT_M1(waitid(P_PID, child_pid, &info, WEXITED | WSTOPPED | WCONTINUED));
			print_code(info.si_code);
			print_status(info.si_status);
			if ((info.si_code==CLD_EXITED) || (info.si_code==CLD_KILLED)) {
				over=true;
			}
		}
	}
	return EXIT_SUCCESS;
}
