#include <stdlib.h> // for EXIT_SUCCESS

/*
 * This application should show how to listen to udev events using API...
 *
 * Why would you want to do this ?
 * - you want a real small RT system and don't want udev on it.
 * - you hate the "system" approach that udev takes to running rules.
 * - you want to listen to udev events in parallel to other things with tight integration.
 * - you want to reduce boot time and want to do things in parallel
 *	(listen to drivers loading while doing other initialization work).
 *
 * Strictly speaking this demo is not a real time demo per se. It is a demo that will
 * usually be used on embedded systems where a full udev is not available but not
 * necessarily a real time system.
 *
 * 				Mark Veltzer
 */

int main(int argc,char** argv,char** envp) {
	return EXIT_SUCCESS;
}
