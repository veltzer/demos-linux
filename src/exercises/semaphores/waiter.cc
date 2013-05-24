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
#include <stdlib.h>	// for EXIT_SUCCESS
#include <sys/types.h>	// for semctl(2), ftok(3), semget(2)
#include <sys/ipc.h>	// for semctl(2), ftok(3), semget(2)
#include <sys/sem.h>	// for semctl(2), semget(2)
#include <us_helper.h>	// for CHECK_NOT_M1(), register_signal_handler(), CHECK_ASSERT()
#include "common.hh"

static bool over=false;

void clean(int sig) {
	over=true;
}

int main(int argc, char** argv, char** envp) {
	register_handler_signal(SIGINT, clean);
	key_t key=CHECK_NOT_M1(ftok(FTOK_PATHNAME, FTOK_PROJID));
	int semid=CHECK_NOT_M1(semget(key, NPHIL, IPC_CREAT | 0666));
	for(unsigned int i=0; i<NPHIL; i++) {
		CHECK_NOT_M1(semctl(semid, i, SETVAL, 1));
	}
	// a non busy busy wait
	while(!over) {
		int ret=pause();
		// this is what is guaranteed by a clean exit
		// of pause(2)
		CHECK_ASSERT(ret==-1 && errno==EINTR);
	}
	// destroy the semaphore
	CHECK_NOT_M1(semctl(semid, 0, IPC_RMID, 0));
	return EXIT_SUCCESS;
}
