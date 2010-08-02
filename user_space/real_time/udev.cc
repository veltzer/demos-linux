/*
 * This application should show how to listen to udev events using API...
 *
 * Why would you want to do this ?
 * - you want a real small RT system and don't want udev on it.
 * - you hate the "system" approach that udev takes to running rules.
 * - you want to listen to udev events in parallel to other things with tight integration.
 * - you want to reduce boot time and want to do things in parallel
 *   	(listen to drivers loading while doing other initialization work).
 */

int main(int argc,char** argv,char** envp) {
	return 0;
}
