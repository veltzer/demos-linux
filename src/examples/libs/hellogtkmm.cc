//$Id: main.cc 836 2007-05-09 03:02:38Z jjongsma $ -*- c++ -*-

/* gtkmm example Copyright (C) 2002 gtkmm development team
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 *			Mark Veltzer
 *
 */

#include <gtkmm/main.h>
#include <gtkmm/button.h>
#include <gtkmm/window.h>
#include <iostream>

/*
 * EXTRA_CMDS=pkg-config --cflags --libs gtkmm-2.4 sigc++-2.0 libgnomeui-2.0
 */

class HelloWorld : public Gtk::Window {
public:
	HelloWorld();
	virtual ~HelloWorld();

protected:
	//Signal handlers:
	void on_button_clicked();

	//Member widgets:
	Gtk::Button m_button;
};

HelloWorld::HelloWorld()
	: m_button("Hello World") {                                                                                                           // creates a new button with label "Hello World".
	// Sets the border width of the window.
	set_border_width(10);

	// When the button receives the "clicked" signal, it will call the
	// on_button_clicked() method defined below.
	m_button.signal_clicked().connect(sigc::mem_fun(*this,
	                                                &HelloWorld::on_button_clicked));

	// This packs the button into the Window (a container).
	add(m_button);

	// The final step is to display this newly created widget...
	m_button.show();
}


HelloWorld::~HelloWorld() {
}


void HelloWorld::on_button_clicked() {
	std::cout << "Hello World" << std::endl;
}


int main(int argc, char** argv, char** envp) {
	Gtk::Main kit(argc, argv);

	HelloWorld helloworld;

	//Shows the window and returns when it is closed.
	Gtk::Main::run(helloworld);

	return(0);
}
