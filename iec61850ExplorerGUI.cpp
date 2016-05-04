#include "ExplorerWindow.h"
#include <QApplication>


int main(int argc, char **argv)
{
	QApplication app (argc, argv);
	ExplorerWindow window;
	window.show();
	return app.exec();
}
