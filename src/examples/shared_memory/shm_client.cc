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
#include <sys/types.h>	// for ftok(3), shmat(2), shmdt(2)
#include <sys/ipc.h>	// for ftok(3), shmget(2), shmctl(2)
#include <sys/shm.h>	// for shmget(2), shmat(2), shmctl(2), shmdt(2)
#include <unistd.h>	// for sleep(3)
#include <time.h>	// for localtime_r(3), strftime(3)
#include <stdlib.h>	// for EXIT_SUCCESS
#include <stdio.h>	// for printf(3)
#include <us_helper.h>	// for CHECK_NOT_M1(), CHECK_NOT_VOIDP(),
			//CHECK_NOT_NULL(), CHECK_NOT_ZERO()

/*
 * This is a simple shared memory client that works well with the shared memory
 * server in shm_server.cc.
 */

struct data {
	pid_t mypid;
	time_t now;
};

int main(int argc, char** argv, char** envp) {
	key_t key=CHECK_NOT_M1(ftok("/etc/passwd", 'x'));
	printf("key is %x (you can see it using ipcs(1))\n", key);
	// remove the old shm, if it exists
	// add IPC_EXCL if you want the following to fail if the shared
	// memory exists...
	int shmid=CHECK_NOT_M1(shmget(key, 0, 0));
	void* ptr=CHECK_NOT_VOIDP(shmat(shmid, NULL, 0), (void*)-1);
	struct data* dateptr=(struct data*)ptr;
	printf("connected to shared memory\n");
	for(int count=0; count<10; count++) {
		struct tm mytm;
		CHECK_NOT_NULL(localtime_r(&dateptr->now, &mytm));
		const unsigned int time_string_length=40;
		char time_string[time_string_length];
		const char* date_repr="%a, %d %b %Y %T %z";
		CHECK_NOT_ZERO(strftime(time_string, time_string_length, date_repr, &mytm));
		printf("%d: Got date %s from process %d\n",
			count,
			time_string,
			dateptr->mypid
			);
		sleep(1);
	}
	CHECK_NOT_M1(shmdt(ptr));
	printf("disconnected from shared memory\n");
	return EXIT_SUCCESS;
}
