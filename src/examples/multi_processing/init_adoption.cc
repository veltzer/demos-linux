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
#include<unistd.h> // for fork(2), getpid(2), sleep(3), getppid(2)
#include<stdio.h> // for fgets(3)
#include<sys/types.h> // for waitid(2), getpid(2), getppid(2)
#include<sys/wait.h> // for waitid(2)
#include<stdlib.h> // for EXIT_SUCCESS
#include<string.h> // for strsignal(3)
#include<signal.h> // for kill(2)

#include<us_helper.h> // for CHECK_NOT_M1()

/*
* This example demostrates what happens when a processes father dies...
*
* - Notice that the getppid() function returns different values because of the
* adoption by the init(1) process.
* - This example forks twice to show that there is no "grandparent adoption" and
* that grandparents are not interested in their grandchildren.
* - We need to wait a little to make sure that the parent dies - there is no synchroneous
* way to get this info as far as I know...
*
* TODO:
* - investigate how it is exactly that init does the things he does (is it the kernel as I
* suspect that latches processes under the init process ?!? - if so, find the place
* in the kernel that does this...).
*/

int main(int argc,char** argv,char** envp) {
	pid_t child_pid;
	CHECK_NOT_M1(child_pid=fork());
	if (child_pid == 0) {
		pid_t gchild_pid;
		CHECK_NOT_M1(gchild_pid=fork());
		if (gchild_pid == 0) {
			TRACE("this is the gchild, pid is %d",getpid());
			// lets show the parent pid...
			TRACE("my parent pid is %d",getppid());
			// now lets signal our parent that its ok to die...
			CHECK_NOT_M1(kill(getppid(),SIGUSR1));
			// lets wait a while to make sure the parent really dies..
			sleep(1);
			// now lets print our parent again...
			TRACE("my parent pid is %d",getppid());
			return 0;
		} else {
			TRACE("this is the parent, pid is %d",getpid());
			// lets wait for a signal that it's ok to die...
			pause();
			// lets die
			return 0;
		}
	} else {
		TRACE("this is the gparent, pid is %d",getpid());
		// lets wait for a signal that it's ok to die...
		pause();
		// lets die
		return 0;
	}
	return EXIT_SUCCESS;
}
