#include "ExplorerWindow.h"
#include <QApplication>
#include <QIcon>

int main(int argc, char **argv)
{
	QApplication app (argc, argv);
	QIcon appIcon;
	appIcon.addFile(":/icons/art/spy");
	app.setWindowIcon(appIcon);
	ExplorerWindow window;
	window.show();
	return app.exec();
}
