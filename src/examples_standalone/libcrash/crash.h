#ifndef CRASH_H
#define CRASH_H

#include <signal.h>

/* Main library registration function. 
 * Call this once for each process (not thread). Recall after fork() if you want
 * process name and ID to be updated 
 * 
 */
int register_crash_handler(
		const char * process_name, 		/* Name of Process, such as argv[0] */
		unsigned char * assert_buf_ptr	/* Pointer to assert buffer */ 
);

/* Asks this thread to dump. You can use this for asserts. */
int static inline crash_dump(void) {
		asm volatile ("" : : : "memory");
        return raise(SIGQUIT);
}

#ifdef USE_THREADS 

/* Ask some other thread to dump. You can use this for asserts. */
int static inline crash_dump_thread(pthread_t thread) {
	asm volatile ("" : : : "memory");
	return pthread_kill(thread, SIGUSR1);
}
#endif /* USE_THREADS */


#endif /* CRASH_H */

