#include <QtGui/qapplication.h>
#include <QtGui/qpushbutton.h>

/*
 * This is a simple Qt hello world program
 *
 * EXTRA_CMDS=pkg-config --cflags --libs QtGui
 */
int main(int argc, char **argv) {
	QApplication app(argc, argv);
	QPushButton hello("Hello, world!", 0);

	QObject::connect(&hello, SIGNAL(clicked()), &app, SLOT(quit()));
	hello.show();
	return(app.exec());
}
