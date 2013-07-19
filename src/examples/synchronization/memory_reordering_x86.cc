/*
 * This file is part of the linuxapi package.
 * Copyright (C) 2011-2013 Mark Veltzer <mark.veltzer@gmail.com>
 *
 * linuxapi is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * linuxapi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with linuxapi. If not, see <http://www.gnu.org/licenses/>.
 */

#include <firstinclude.h>
#include <stdio.h>	// for printf(3)
#include <stdlib.h>	// for EXIT_SUCCESS, EXIT_FAILURE, rand(3)
#include <pthread.h>	// for pthread_t, pthread_attr_t, pthread_create(3), pthread_attr_init(3), pthread_attr_destroy(3), pthread_attr_setaffinity_np(3)
#include <sched.h>	// for cpu_set_t, CPU_ZERO(3), CPU_SET(3)
#include <semaphore.h>	// for sem_t:struct, sem_post(3), sem_wait(3), sem_init(3)
#include <err_utils.h>	// for CHECK_ZERO_ERRNO()
#include <us_helper.h>	// for no_params()
#include <random_utils.hh>	// for MersenneTwister:object
#include <disassembly_utils.h>	// for disassemble_function()

/*
 * This example explores memory reordering issues by the machine.
 *
 * Compile this with USE_CPU_FENCE defined to see the reordering
 * issues go away...
 *
 * References:
 * http://preshing.com/20120515/memory-reordering-caught-in-the-act
 *
 * So that we could disassemble this easily...
 * EXTRA_COMPILE_FLAGS=-g3
 *
 * EXTRA_LINK_FLAGS=-lpthread
 */

#define USE_CPU_FENCE 1
#define USE_SYNC_SYNCHRONIZE 0
#define USE_COMPILER_BARRIER 0
#define PUT_ON_SAME_CORE 0

sem_t beginSema1;
sem_t beginSema2;
sem_t endSema;
int X, Y;
int r1, r2;

void *thread1Func(void *param) {
	MersenneTwister random(1);
	while(true) {
		sem_wait(&beginSema1);	// Wait for signal
		while (random.integer() % 8 != 0) {
		}	// Random delay

		// ----- THE TRANSACTION! -----
		X = 1;
#if USE_CPU_FENCE
		asm volatile ("mfence" ::: "memory");	// Prevent CPU reordering
#endif
#if USE_SYNC_SYNCHRONIZE
		__sync_synchronize();
#endif
#if USE_COMPILER_BARRIER
		asm volatile ("" ::: "memory");	// Prevent compiler reordering
#endif
		r1 = Y;

		sem_post(&endSema);	// Notify transaction complete
	}
	return NULL;	// Never returns
};

void *thread2Func(void *param) {
	MersenneTwister random(2);
	while(true) {
		sem_wait(&beginSema2);	// Wait for signal
		while (random.integer() % 8 != 0) {
		}	// Random delay

		// ----- THE TRANSACTION! -----
		Y = 1;
#if USE_CPU_FENCE
		asm volatile ("mfence" ::: "memory");	// Prevent CPU reordering
#endif
#if USE_SYNC_SYNCHRONIZE
		__sync_synchronize();
#endif
#if USE_COMPILER_BARRIER
		asm volatile ("" ::: "memory");	// Prevent compiler reordering
#endif
		r2 = X;

		sem_post(&endSema);	// Notify transaction complete
	}
	return NULL;	// Never returns
};

int main(int argc, char** argv, char** envp) {
	no_params(argc, argv);
	// show some disassembly...
	// disassemble_function("thread1Func");
	// disassemble_function("thread2Func");
	// Initialize the semaphores
	CHECK_ZERO_ERRNO(sem_init(&beginSema1, 0, 0));
	CHECK_ZERO_ERRNO(sem_init(&beginSema2, 0, 0));
	CHECK_ZERO_ERRNO(sem_init(&endSema, 0, 0));

	// Spawn the threads
	cpu_set_t cpuset1;
	CPU_ZERO(&cpuset1);
	CPU_SET(0, &cpuset1);
	pthread_attr_t attrs1;
	CHECK_ZERO_ERRNO(pthread_attr_init(&attrs1));
	CHECK_ZERO_ERRNO(pthread_attr_setaffinity_np(&attrs1, sizeof(cpu_set_t), &cpuset1));
	pthread_t thread1;
	CHECK_ZERO_ERRNO(pthread_create(&thread1, &attrs1, thread1Func, NULL));
	CHECK_ZERO_ERRNO(pthread_attr_destroy(&attrs1));

	cpu_set_t cpuset2;
	CPU_ZERO(&cpuset2);
#if PUT_ON_SAME_CORE
	CPU_SET(0, &cpuset2);
#else
	CPU_SET(1, &cpuset2);
#endif
	pthread_attr_t attrs2;
	CHECK_ZERO_ERRNO(pthread_attr_init(&attrs2));
	CHECK_ZERO_ERRNO(pthread_attr_setaffinity_np(&attrs2, sizeof(cpu_set_t), &cpuset2));
	pthread_t thread2;
	CHECK_ZERO_ERRNO(pthread_create(&thread2, &attrs2, thread2Func, NULL));
	CHECK_ZERO_ERRNO(pthread_attr_destroy(&attrs2));

	// Repeat the experiment ad infinitum
	int detected = 0;
	int iterations = 1;
	while(true) {
		// Reset X and Y
		X = 0;
		Y = 0;
		// Signal both threads
		CHECK_ZERO_ERRNO(sem_post(&beginSema1));
		CHECK_ZERO_ERRNO(sem_post(&beginSema2));
		// Wait for both threads
		CHECK_ZERO_ERRNO(sem_wait(&endSema));
		CHECK_ZERO_ERRNO(sem_wait(&endSema));
		// Check if there was a simultaneous reorder
		if (r1 == 0 && r2 == 0) {
			detected++;
			printf("%d simulteneous reorders detected after %d iterations\n", detected, iterations);
		}
		iterations++;
	}
	return EXIT_SUCCESS;
}
