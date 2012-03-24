#include <stdio.h> // for fprintf(3)
#include <stdlib.h> // for atexit(3), on_exit(3), exit(3)
#include <unistd.h> // for sysconf(3)

/*
 * This example shows how to use the cleanup framework provided by the standard C library
 * notice that cleanup functions are called in reverse order of their registration
 *
 * Notes:
 * - the same function can be registered multiple times (with atexit or on_exit).
 * - the limit to number of registrations seems to be very high (millions ?!?).
 * - the API to on_exit seems to be a bit better since it allows to pass a pointer
 * and get the exit code which atexit does not allow.
 * - on the other hand the documentation states that atexit is preffered on Linux.
 *
 * 			Mark Veltzer
 */

void endfunc1() {
	fprintf(stderr,"Hey! I am doing some cleanup work 1\n");
}
void endfunc2() {
	fprintf(stderr,"Hey! I am doing some cleanup work 2\n");
}
void endfunc3(int exit_code,void* ptr) {
	fprintf(stderr,"Hey! I am doing some cleanup work 3\n");
	fprintf(stderr,"exit_code is %d\n",exit_code);
	fprintf(stderr,"ptr is %p\n",ptr);
}

int main(int argc,char** argv,char** envp) {
	// lets find out how many atexit functions can we register at a maximum...
	long val_atexit_max=sysconf(_SC_ATEXIT_MAX);
	fprintf(stderr,"ATEXIT_MAX is %ld\n",val_atexit_max);
	// lets register some functions...
	atexit(endfunc1);
	atexit(endfunc1);
	atexit(endfunc2);
	on_exit(endfunc3,NULL);
	on_exit(endfunc3,NULL);
	// this will end the software immediately
	// _exit(7);
	exit(17);
	return(0);
}
