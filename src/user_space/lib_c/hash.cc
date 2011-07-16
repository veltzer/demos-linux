#include <stdio.h>
#include <string.h>
#include <search.h>

#include "us_helper.hh"

/*
 *      This demos how to use the has supplied with the standard C library...
 *
 *      You can see documentation for these functions at
 *      man hsearch or info libc under 'hsearch' or 'hash'.
 *
 *              Mark Veltzer
 *
 * EXTRA_LIBS=
 */
int main(int argc, char **argv, char **envp) {
	//struct hsearch_data HTAB;
	//SC(hcreate_r(50,&HTAB));
	// next function does not return any status...
	//hdestroy_r(&HTAB);
	SC(hcreate(50));
	// next function does not return any status...
	hdestroy();
	// TODO
	return(0);
}
