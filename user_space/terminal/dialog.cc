#include <stdio.h>
#include <string.h>
//extern "C" {
#include <dialog.h>
//}

/*
 *      This example shows how to use the dialog C library.
 *
 * 				Mark Veltzer
 *
 * EXTRA_LIBS=/usr/lib/libdialog.a -lncursesw
 *
 * TODO:
 * - you may want to write this ?!?
 */
int main(int argc, char **argv, char **envp) {
	init_dialog(NULL, NULL);
	end_dialog();
	return(0);
}
