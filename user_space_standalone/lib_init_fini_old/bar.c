#include <stdio.h>

void my_init(void) {
	// call the original...
	_init();
	fprintf(stderr, "I'm in my_init %s\n", __FILE__);
}


void my_fini(void) {
	fprintf(stderr, "I'm in my_fini %s\n", __FILE__);
	// call the original...
	_fini();
}
