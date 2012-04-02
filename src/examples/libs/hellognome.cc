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

#include<firstinclude.h>
#include<gnome.h> // for gnome_init(3), gnome_app_new(3), gtk_widget_show(3), gtk_main(3)

/*
 * This is a demo program for using C Gnome
 *
 * EXTRA_CMDS=pkg-config --cflags --libs libgnomeui-2.0
 */

int main(int argc,char** argv,char** envp) {
	gnome_init("mdi-test", "1.0", argc, argv);
	GtkWidget *w = gnome_app_new("mdi-test", "1.0");
	gtk_widget_show(w);
	gtk_main();
	return EXIT_SUCCESS;
}
