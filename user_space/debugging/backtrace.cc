#include <stdio.h>
#include <execinfo.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

/*
 *      This exapmle shows how to obtain a stack trace for various purposes (mostly
 *      for 'self debugging...'...).
 *
 *      Some notes:
 *      - you DO NOT have to compile with debugging information. You will see any
 *      dynamic symbols (for all you DLL lovers - this means every exported symbol
 *      of the DLL which in linux means everything you didn't put 'static' on...
 *      - for this example I do compile with -rdynamic to get the symbol names.
 *      - this example does not do or show cxx name mangling.
 *      - this example also shows how to register a signal handler for the SIGSEGV
 *      signal in order to dump stack traces...
 *
 *              Mark Veltzer
 *
 * EXTRA_LIBS=
 */

/*
 *      Please keep this a C function, as it should be usable for C infrastructure as well...
 */
void print_trace(void) {
	unsigned int nptrs;
	const unsigned int MAX_FRAMES = 100;
	void               *buffer[MAX_FRAMES];

//	char **strings;
//	unsigned int j;
	nptrs = backtrace(buffer, MAX_FRAMES);
	fprintf(stderr, "backtrace() returned %d addresses\n", nptrs);
	backtrace_symbols_fd(buffer, nptrs, fileno(stderr));

	/*
	 *      strings = backtrace_symbols(buffer, nptrs);
	 *      if (strings == NULL) {
	 *              perror("backtrace_symbols");
	 *              exit(EXIT_FAILURE);
	 *      }
	 *      for (j = 0; j < nptrs; j++)
	 *              fprintf(stderr,"%s\n", strings[j]);
	 *      free(strings);
	 */
}


//void print_trace(void) __attribute__ ((__noinline__));

/*
 *      static old handler so that we would call the old handling code...
 */
typedef void (*sighandler_t)(int);
static sighandler_t old_handler;

/*
 *      Wrapper for signal handling
 */
//void print_trace_sighandler(int sig, siginfo_t * siginf, void * context) {
void print_trace_sighandler(int sig) {
	fprintf(stderr, "got sig %d\n", sig);
	print_trace();
	(*old_handler)(sig);
}


/*
 *      Signal handler registration function
 */
void trace_register(void) {
	old_handler = signal(SIGSEGV, print_trace_sighandler);
	if (old_handler == SIG_ERR) {
		perror("could not register signal...");
		exit(1);
	}

	/*
	 *      struct sigaction sa;
	 *      memset(sa,0,sizeof(struct sigaction));
	 */
}


/*
 *      Simple function that generates a segmentation fault...
 */
void do_fault(void) {
	char *p = (char *)0;

	p[0] = 0;
}


/*
 *      I use several ways to try to force the compiler not to flatten the next function:
 *      1. use asm("").
 *      2. __attribtue__ (( __noinline__ ));
 *      3. complicating the function code with an integer accumulator and recursion.
 *      finally it worked but I think it is number (3) that made it happen...
 *      Need to investigate this further...
 */
int rec_func(unsigned int ncalls, unsigned int ret) {
	printf("ncalls is %d\n", ncalls);
	ret += ncalls;
	asm ("");
	if (ncalls > 1) {
		rec_func(ncalls - 1, ret);
	} else {
		//print_trace();
		do_fault();
	}
	return(ret);
}


int rec_func(unsigned int ncalls, unsigned int ret) __attribute__((__noinline__));

int main(int argc, char **argv, char **envp) {
	unsigned int num;

	trace_register();
	if (argc > 1) {
		num = atoi(argv[1]);
	} else {
		num = 3;
	}
	rec_func(num, 0);
	return(0);
}
