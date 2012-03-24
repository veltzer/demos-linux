#include <gtkmm/main.h> // for Gtk::Main
#include <gtkmm/button.h> // for Gtk::Button
#include <gtkmm/window.h> // for Gtk::Window
#include <iostream> // for std::cout, std::endl

/*
 * A stolen example from the gtkmm distribution. Slightly modified.
 *
 *	Mark Veltzer
 *
 * EXTRA_CMDS=pkg-config --cflags --libs gtkmm-2.4 sigc++-2.0 libgnomeui-2.0
 */

class HelloWorld:public Gtk::Window {
	public:
		HelloWorld();
		virtual ~HelloWorld();
	protected:
		//Signal handlers:
		void on_button_clicked();
		//Member widgets:
		Gtk::Button m_button;
};

HelloWorld::HelloWorld():
	m_button("Hello World") // creates a new button with label "Hello World".
{
	// Sets the border width of the window.
	set_border_width(10);

	// When the button receives the "clicked" signal, it will call the
	// on_button_clicked() method defined below.
	m_button.signal_clicked().connect(sigc::mem_fun(*this, &HelloWorld::on_button_clicked));

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
