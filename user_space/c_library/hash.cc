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
	//SCIE(hcreate_r(50,&HTAB),"hcreate_r");
	// next function does not return any status...
	//hdestroy_r(&HTAB);
	SCIE(hcreate(50), "hcreate");
	// next function does not return any status...
	hdestroy();
	// TODO
	return(0);
}
