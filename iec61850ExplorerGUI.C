#include <QApplication>
#include <QPushButton>
#include <QTableWidget>
#include <QListView>
#include <QLabel>
#include <QGridLayout>

#include "iec61850ExplorerGUI.h"


ExplorerWindow::ExplorerWindow(QWidget *parent) : QWidget(parent)
{
	QLabel *labelServer = new QLabel(tr("&Server:"));
	lineEditServer = new QLineEdit;
	labelServer->setBuddy(lineEditServer);

	QLabel *labelPort = new QLabel(tr("&Port:"));
	lineEditPort = new QLineEdit;
	labelPort->setBuddy(lineEditPort);

	QPushButton * browse = new QPushButton(tr("&Browse"));
	connect(browse, SIGNAL(clicked()), this, SLOT(onBrowse()));

	textViewer = new QTextBrowser;
	textViewer->setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred));

	QGridLayout *layout = new QGridLayout;
	layout->addWidget(labelServer, 0, 0);
	layout->addWidget(lineEditServer, 0, 1);
	layout->addWidget(labelPort, 1, 0);
	layout->addWidget(lineEditPort, 1, 1);
	layout->addWidget(browse, 2, 0, 1, 2);
	layout->addWidget(textViewer, 3, 0, 1, 2);

	setLayout(layout);
	setWindowTitle(tr("IEC61850 Explorer"));
}

void ExplorerWindow::onBrowse()
{
	textViewer->setText("Hello world");

}

int main(int argc, char **argv)
{
	QApplication app (argc, argv);
	ExplorerWindow window;
	window.show();
	return app.exec();
}
