/*
 * This is a demo program showing how to use libcwd for C++ debug support...
 *
 *			Mark Veltzer
 *
 * EXTRA_LIBS=-lcwd
 */
#define DEBUG
#ifdef DEBUG
#define CWDEBUG
#include <libcwd/sys.h>
#include <libcwd/debug.h>
// Define our own debug channel:
namespace libcwd {
	namespace channels {
		namespace dc {
			channel_ct hello("HELLO");
		};
	};
};
#endif

//using namespace libcwd::channels::dc;

int main(int argc, char** argv, char** envp) {
	Debug(check_configuration());
	Debug(libcw_do.on());
	// List all debug channels (nor very usefull unless you allow to turn
	// channels on and off from the commandline; this is supported in libcw).
	Debug(list_channels_on(libcw_do));
	Debug(dc::hello.on());
	Dout(dc::hello,"==============================");
	Debug(list_channels_on(libcw_do));
	Dout(dc::hello,"Hello World!");
	return(0);
}
