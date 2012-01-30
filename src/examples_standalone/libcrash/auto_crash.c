#include <stdio.h>
#include "crash.h"

/* OK, we want a way to call the register function (not to mention load the library)
 * without messing with source files and re-compiling. Here how it works:
 * 
 * We create a library with a single init function that calls the registration for us
 * using a function constructor hook and we'll force the dynamic linker to loda it
 * in run time using LD_PRELOAD magic.
 * 
 * Pretty, clever, isn't it? :-)
 */

void __attribute__ ((constructor)) debug_auto_init(void);

/* Buffer for assert info that we need to supply */
static unsigned char buf[128];

void debug_auto_init(void) {
	
	/* Print some meaningfull message so they'll know that we're running */
	fprintf(stderr, "\n*** libcrash automagically loaded. Registering...");
	
	/* Blast us if we know what's the process name. We just use [AUTOMAGIC] */
	
	if(register_crash_handler("[AUTOMAGIC]", buf))
		fprintf(stderr, "Failed!\n");
	else
		fprintf(stderr, "Success.\n");
	
	return;
}

/* THE END */
