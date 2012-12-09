/*
	This file is part of the linuxapi project.
	Copyright (C) 2011, 2012 Mark Veltzer <mark.veltzer@gmail.com>

	The linuxapi package is free software; you can redistribute it and/or
	modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 2.1 of the License, or (at your option) any later version.

	The linuxapi package is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public
	License along with the GNU C Library; if not, write to the Free
	Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
	02111-1307 USA.
*/

#include <firstinclude.h>
#include <libgnomeuimm.h> // for Glib::OptionContext(O), Gnome::Main(O), Gnome::UI::App(O)
#include <stdlib.h> // for EXIT_SUCCESS

/*
* This is a demo program for using Gnome--
*
* EXTRA_CMDS=pkg-config --libs --cflags libgnomeuimm-2.6
*/

int main(int argc,char** argv,char** envp) {
	Glib::OptionContext context;
	Gnome::Main app("gnome-hello", "0.1",
		Gnome::UI::module_info_get(), argc, argv,
		context);
	Gnome::UI::App *a=new Gnome::UI::App("foo", "bar");
	a->show();
	app.run();
	return EXIT_SUCCESS;
}
