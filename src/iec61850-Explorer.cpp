/// @author Cedric Boudinet
/// @copyright GNU GPL Version 3
///
/// Distributed under the GNU GPL version 3 License
/// (See accompanying file LICENSE or copy at
/// http://www.gnu.org/licenses/)
///
#include "ExplorerWindow.h"
#include <QApplication>
#include <QIcon>
/**
	iec61850 main function
*/
int main(int argc, char **argv)
{
	QApplication app (argc, argv);
	QIcon appIcon;
	appIcon.addFile(":/icons/art/spy");
	app.setWindowIcon(appIcon);
	QString host="localhost";
	QString port="102";
	if(argc>1)
		host=argv[1];
	if(argc>2)
		port=argv[2];
	ExplorerWindow window(host, port);
	window.show();
	return app.exec();
}
