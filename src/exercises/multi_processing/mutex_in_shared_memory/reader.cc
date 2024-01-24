/*
 * This file is part of the demos-linux package.
 * Copyright (C) 2011-2024 Mark Veltzer <mark.veltzer@gmail.com>
 *
 * demos-linux is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * demos-linux is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with demos-linux. If not, see <http://www.gnu.org/licenses/>.
 */

#include <firstinclude.h>
#include <err_utils.h>
#include <measure.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>

int main(int argc, char** argv, char** envp) {
	int shm_fd=CHECK_NOT_M1(shm_open("data", O_CREAT | O_RDWR, 0666));
	CHECK_NOT_M1(ftruncate(shm_fd, sizeof(int)+sizeof(pthread_mutex_t)) == -1);
	int* data=(int*)CHECK_NOT_VOIDP(mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0), MAP_FAILED);
	pthread_mutex_t* lock=(pthread_mutex_t*)(data+1);
	int counter=0;
	measure m;
	measure_init(&m, "wait for lock", 1);
	while(true) {
		measure_start(&m);
		CHECK_ZERO_ERRNO(pthread_mutex_lock(lock));
		measure_end(&m);
		measure_print(&m);
		printf("reader lock\n");
		memcpy(&counter, data, sizeof(int));
		printf("reading %d\n", counter);
		printf("reader unlock\n");
		CHECK_ZERO_ERRNO(pthread_mutex_unlock(lock));
		sleep(1);
	}
	return EXIT_SUCCESS;
}
