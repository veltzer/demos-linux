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
#include <sys/types.h>	// for ftok(3), shmat(2)
#include <sys/ipc.h>	// for ftok(3), shmget(2), shmctl(2)
#include <sys/shm.h>	// for shmget(2), shmat(2), shmctl(2)
#include <unistd.h>	// for sleep(3)
#include <time.h>	// for time(2)
#include <stdlib.h>	// for EXIT_SUCCESS
#include <stdio.h>	// for printf(3)
#include <signal.h>	// for signal(2)
#include <us_helper.h>	// for CHECK_NOT_M1(), CHECK_NOT_VOIDP(),
			// CHECK_NOT_SIGT()

/*
 * This is a simple shared memory server that works well with the shared memory
 * client in shm_client.cc.
 */

struct data {
	pid_t mypid;
	time_t now;
};
bool cont=true;

void sigterm_exit(int sig) {
	printf("got signal, signaling to quit\n");
	cont=false;
}

int main(int argc, char** argv, char** envp) {
	// first lets make sure that we exit if we get SIGINT
	CHECK_NOT_SIGT(signal(SIGINT, sigterm_exit), SIG_ERR);
	key_t key=CHECK_NOT_M1(ftok("/etc/passwd", 'x'));
	printf("key is 0x%x\n", key);
	// remove the old shm, if it exists
	// add IPC_EXCL if you want the following to fail if the shared
	// memory exists...
	int shmid=CHECK_NOT_M1(shmget(key, sizeof(struct data), IPC_CREAT|IPC_EXCL|0666));
	printf("shmid is %d\n", shmid);
	void* ptr=CHECK_NOT_VOIDP(shmat(shmid, NULL, 0), (void*)-1);
	struct data* dateptr=(struct data*)ptr;
	int count=0;
	printf("shared memory was created, you can now run the client\n");
	printf("you can see info about the shared memory using \"ipcs -m -i %d\"\n", shmid);
	printf("you can remove the shared memory using \"ipcrm -M 0x%x\"\n", key);
	printf("or using \"ipcrm -m %d\"\n", shmid);
	while(cont) {
		dateptr->mypid=getpid();
		dateptr->now=time(NULL);
		sleep(1);
		printf("updated the shared memory (%d)\n", count);
		count++;
	}
	CHECK_NOT_M1(shmdt(ptr));
	printf("disconnected from shared memory\n");
	CHECK_NOT_M1(shmctl(shmid, IPC_RMID, NULL));
	printf("removed the shared memory\n");
	return EXIT_SUCCESS;
}
