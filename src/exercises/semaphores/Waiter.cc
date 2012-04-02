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
#include"Phil.hh"
#include<stdlib.h> // for EXIT_SUCCESS

int semid;

void clean(int sig)
{
	int rc=semctl(semid,0,IPC_RMID,0);
	if(rc) {
		perror("error in semctl");
		exit(errno);
	} else {
		exit(0);
	}
}

int main(int argc,char** argv,char** envp) {
	int i;
	key_t key;
	signal(SIGINT,clean);
	if ((key = ftok(KEYFILE, 'x')) == -1) {
		perror("ftok failed");
		exit(errno);
	}
	if ((semid = semget(key, NPHIL, IPC_CREAT | 0666)) < 0 ) {
		perror("semget");
		exit(errno);
	}
	for(i=0; i<NPHIL; i++) {
		if (semctl(semid, i,SETVAL,1 ) < 0) {
			perror("semctl 1");
			exit(errno);
		}
	}
	while(true)
		pause();
	return EXIT_SUCCESS;
}
