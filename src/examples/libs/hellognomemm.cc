#include <libgnomeuimm.h>

/*
 *      This is a demo program for using Gnome--
 *
 *			Mark Veltzer
 *
 * EXTRA_CMDS=pkg-config --libs --cflags libgnomeuimm-2.6
 */
int main(int argc, char **argv, char **envp) {
	Glib::OptionContext context;

	Gnome::Main app("gnome-hello", "0.1",
	                Gnome::UI::module_info_get(), argc, argv,
	                context);

	Gnome::UI::App *a = new Gnome::UI::App("foo", "bar");

	a->show();
	app.run();
	return(0);
}
