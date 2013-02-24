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
#include <stdlib.h> // for EXIT_SUCCESS, exit(3), EXIT_FAILURE, rand(3), srand(3)
#include <time.h> // for time(2)
#include <unistd.h> // for sleep(3)
#include <stdio.h> // for printf(3)
#include <sys/types.h> // for semop(2), ftok(3), semget(2)
#include <sys/ipc.h> // for semop(2), ftok(3), semget(2)
#include <sys/sem.h> // for semop(2), semget(2)
#include <us_helper.h> // for CHECK_NOT_M1()
#include "phil.hh"

static int semid;

void think(int id) {
	int stime;
	printf("Philosopher %d is busy thinking\n", id);
	srand(time(NULL));
	stime=1+(int) (10.0*rand()/(RAND_MAX+1.0));
	sleep(stime);
	printf("Philosopher %d finished thinking\n", id);
}

void eat(int id) {
	int stime;
	printf("Philosopher %d is busy eating\n", id);
	srand(time(NULL));
	stime=1+(int) (10.0*rand()/(RAND_MAX+1.0));
	sleep(stime);
	printf("Philosopher %d finished eating\n", id);
}

void putForks(int id) {
	int next;
	if(id==NPHIL-1)
		next=0;
	else
		next=id+1;
	printf("Philosopher %d is replacing the forks\n", id);
	struct sembuf sops[2];
	sops[0].sem_num=id;
	sops[0].sem_op=1;
	sops[0].sem_flg=0;
	sops[1].sem_num=next;
	sops[1].sem_op=1;
	sops[1].sem_flg=0;
	CHECK_NOT_M1(semop(semid, sops, 2));
	printf("Philosopher %d replaced the forks\n", id);
}

void pickForks(int id) {
	int next;
	if(id==NPHIL-1)
		next=0;
	else
		next=id+1;
	printf("Philosopher %d is trying to pick up the forks\n", id);
	struct sembuf sops[2];
	sops[0].sem_num=id;
	sops[0].sem_op=-1;
	sops[0].sem_flg=0;
	sops[1].sem_num=next;
	sops[1].sem_op=-1;
	sops[1].sem_flg=0;
	CHECK_NOT_M1(semop(semid, sops, 2));
	printf("Philosopher %d finally picked the forks\n", id);
}

int main(int argc,char** argv,char** envp) {
	key_t key;
	int id;
	if(argc!=2) {
		fprintf(stderr, "Usage: %s [0-%d]\n", argv[0], NPHIL-1);
		exit(EXIT_FAILURE);
	}
	if((argv[1][0]>='0') && (argv[1][0] <='5') && (strlen(argv[1])==1))
		id=atoi(argv[1]);
	else {
		fprintf(stderr, "bad argument %c, Argument must be numeric value between 0 and 5\n", argv[1][0]);
		exit(EXIT_FAILURE);
	}

	key=CHECK_NOT_M1(ftok(KEYFILE, 'x'));

	semid=CHECK_NOT_M1(semget(key, 0, 0));

	while(true) {
		pickForks(id);
		eat(id);
		putForks(id);
		think(id);
	}
	return EXIT_SUCCESS;
}
