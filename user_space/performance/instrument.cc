#include<unistd.h> // for sleep(3)
#include<stdio.h> // for printf(3)

/*
 * This is a demo of how to use the instrumentation feature of the gnu compiler.
 *
 * Notes:
 * - notice that the flags passed to the compiler ask it not to instrument the instrumenting
 *   functions for obvious infinite recursion reasons.
 * - If you, for instance, use your own debug functions inside the the instrumentation
 * functions here and these debug functions are also instrumented then you will get
 * an infinited recursion.
 * - This is not a big problem in practice since the instrumentation subsystem of a large
 *   system is usually a separate piece of code which is compiled sepertely, does not call
 *   code from the main code and is compiled without the instrumentation flags.
 * - It seems that printf is inlined also and so you have to tell the compiler not to instrument
 *   that as well.
 * - Since I do not wish to instrument main then I add it to the flags as well.
 *
 *              Mark Veltzer
 *
 * EXTRA_LIBS=-finstrument-functions -finstrument-functions-exclude-function-list=__cyg_profile_func_enter,__cyg_profile_func_exit,printf,main
 */
void long_task(void) {
	sleep(1);
}

extern "C" void __cyg_profile_func_enter(void *this_fn, void *call_site) {
	printf("entering %p,%p\n",this_fn,call_site);
}
extern "C" void __cyg_profile_func_exit(void *this_fn, void *call_site) {
	printf("exiting %p,%p\n",this_fn,call_site);
}

int main(int argc, char **argv, char **envp) {
	long_task();
	return(0);
}
