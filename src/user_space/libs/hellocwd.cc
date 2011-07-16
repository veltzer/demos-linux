//#include <libcw/sysd.h>

/*
 #ifdef DEBUG
 #define CWDEBUG
 #include <libcw/debug.h>
 #endif
 *
 #ifdef DEBUG
 * // Define our own debug channel:
 * namespace libcw {
 *      namespace debug {
 *              namespace channels {
 *                      namespace dc {
 *                              channel_ct const hello("HELLO");
 *                      };
 *              };
 *      };
 * };
 #endif
 */
int main(int argc, char **argv) {
	/*
	 * Debug(check_configuration());
	 * Debug( libcw_do.on());
	 */
	// List all debug channels (nor very usefull unless you allow to turn
	// channels on and off from the commandline; this is supported in libcw).

	/*
	 * Debug(list_channels_on(libcw_do));
	 * Dout(libcw::debug::channels::dc::hello,"Hello World!");
	 */
	return(0);
}
