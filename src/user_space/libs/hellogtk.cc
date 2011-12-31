#include <gtk/gtk.h>

/*
 * This is a simple hello gtk program
 * EXTRA_CMDS=pkg-config --cflags --libs gtk+-2.0
 *
 *			Mark Veltzer
 *
 */
int main(int argc, char** argv, char** envp) {
	gtk_init(&argc, &argv);
	GtkWidget *w = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_widget_show(w);
	gtk_main();
	return(0);
}
