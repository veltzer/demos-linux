#include <stdio.h>

extern char *build_id;

void my_init(void) {
	// call the original...
	_init();
	fprintf(stderr, "I'm in my_init %s\n", __FILE__);
	// how do I get the build id from my own object ? This does not work
	//extern char* build;
	//fprintf(stderr, ".note.gnu.build-id is %s", build);
}


void my_fini(void) {
	fprintf(stderr, "I'm in my_fini %s\n", __FILE__);
	// call the original...
	_fini();
}
