/* This is a demo program for using C Gnome */

/*
 * EXTRA_CMDS=pkg-config --cflags --libs libgnomeui-2.0
 */

#include <gnome.h>

int main(int argc, char **argv) {
	gnome_init("mdi-test", "1.0", argc, argv);
	GtkWidget *w = gnome_app_new("mdi-test", "1.0");
	gtk_widget_show(w);
	gtk_main();
	return(0);
}
