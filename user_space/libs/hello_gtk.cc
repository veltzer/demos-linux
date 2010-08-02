#include <gtk/gtk.h>
//#include<stdio.h>

/*
 *      This is an simple gtk application.
 *
 *                              Mark Veltzer
 * EXTRA_LIBS=
 * EXTRA_CMDS=pkg-config --cflags --libs gtk+-2.0
 */
int main(int argc, char **argv, char **envp) {
	gtk_init(&argc, &argv);
	GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_widget_show(window);
	gtk_main();
	return(0);
}


/*
 *      These are a few functions I tried to use to solve static linking issues...
 */

/*
 * extern "C" void th_brk(void) {
 *      fprintf(stderr,"In here\n");
 * }
 * extern "C" void th_uni2tis(void) {
 *      fprintf(stderr,"In here\n");
 * }
 */
