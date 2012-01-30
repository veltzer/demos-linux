#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include <execinfo.h>
#include <string.h>
#include "demangle.hh"

/*
 * This is an example of using setjmp/longjmp to simulate exception handling
 * in C code. This example expands on the previous example in that it adds stack
 * traces to the "errors" which are generated.
 *
 * 				Mark Veltzer
 */

const int max_stack_frames = 25;
const int drop_frames_start = 3;
const int drop_frames_end = 1;
const int max_message_size = 256;

struct error_data {
	int size;
	void *array[max_stack_frames];
	char **symbols;
	char message[max_message_size];
};
// The last error reported
static error_data *last_error;
// This buffer saves the context of the jump. We need one for each unique
// point we wish to jump to
static jmp_buf env;

inline void error_create(const char *message) {
	error_data *p = (error_data *)malloc(sizeof(error_data));

	p->size = backtrace(p->array, max_stack_frames);
	p->symbols = backtrace_symbols(p->array, p->size);
	strncpy(p->message, message, max_message_size);
	last_error = p;
	longjmp(env, (int)p);
}


inline void error_print(FILE *f, error_data *p) {
	fprintf(f, "error message is [%s]\n", p->message);
	for (int i = p->size - drop_frames_start; i >= drop_frames_end; i--) {
		char *symbol = p->symbols[i];
		char result_name[256];
		char result_offset[256];
		error_demangle(symbol, result_name, 256, result_offset, 256);
		fprintf(f, "%s,%s,%s\n", symbol, result_name, result_offset);
	}
}


inline void error_print_last(FILE *f) {
	error_print(f, last_error);
}


inline void error_free(error_data *p) {
	free(p->symbols);
	free(p);
}


inline void error_free_last() {
	error_free(last_error);
}


// This function **must** be inlined as if setjmp returns then env will no longer
// be valid. That's why we don't use it (there is no way to guarantee inlining).
inline error_data *error_setjmp() {
	int ret = setjmp(env);

	if (!ret) {
		return(NULL);
	} else {
		return((error_data *)ret);
	}
}


// here is another idea: why not use a macro instead:
#define mac_error_setjmp() ((error_data *)setjmp(env))

/* this simulates a function which sometimes encounters errors */
void func() {
	static int counter = 0;

	counter++;
	if (counter % 3 == 0) {
		error_create("some error");
	}
	fprintf(stderr, "this is the continuation of the function\n");
}


int main(int argc, char **argv, char **envp) {
	for (int c = 0; c < 10; c++) {
		//int ret=setjmp(env);
		//error_data* p=(error_data*)ret;
		//error_data* p=error_setjmp();
		error_data *p = mac_error_setjmp();
		if (p == NULL) {
			// This is the regular code. We get here when setting doing the
			// setjmp for the first time
			fprintf(stderr, "c is %d\n", c);
			func();
		} else {
			// we got an error
			error_print(stderr, p);
			error_free(p);
		}
	}
	return(0);
}
