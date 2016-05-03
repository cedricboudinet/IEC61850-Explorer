#include <QApplication>
#include <QPushButton>
#include <QTableWidget>
#include <QListView>
#include <QLabel>
#include <QGridLayout>
#include <QMessageBox>

#include "iec61850ExplorerGUI.h"
#include <iostream>
#include "VariablesListWindow.h"
#include "iec61850Exp_version.h"


ExplorerWindow::ExplorerWindow(QWidget *parent) : QWidget(parent)
{
	QLabel *labelServer = new QLabel(tr("&Server:"));
	lineEditServer = new QLineEdit("localhost");
	labelServer->setBuddy(lineEditServer);

	QLabel *labelPort = new QLabel(tr("&Port:"));
	lineEditPort = new QLineEdit("102");
	lineEditPort->setValidator(new QIntValidator(1, 0xFFFF));
	labelPort->setBuddy(lineEditPort);

	connectBtn = new QPushButton(tr("&Connect"));
	connect(connectBtn, SIGNAL(clicked()), this, SLOT(onConnect()));

	addVar = new QPushButton(tr("&Add variables"));
	addVar->setEnabled(false);
	connect(addVar, SIGNAL(clicked()), this, SLOT(onAddVar()));

	textViewer = new QTextBrowser;
	textViewer->setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred));

	QGridLayout *layout = new QGridLayout;
	layout->addWidget(labelServer, 0, 0);
	layout->addWidget(lineEditServer, 0, 1);
	layout->addWidget(labelPort, 1, 0);
	layout->addWidget(lineEditPort, 1, 1);
	layout->addWidget(connectBtn, 2, 0, 1, 1);
	layout->addWidget(addVar, 2, 1, 1, 1);
	layout->addWidget(textViewer, 3, 0, 1, 2);

	setLayout(layout);
	std::string title = "IEC61850 Explorer v ";
	title+=IECEXP_VERSION;
	setWindowTitle(title.c_str());
	IedCon = IedConnection_create();
}

void ExplorerWindow::onAddVar()
{
	std::cout<<"Adding variables"<<std::endl;
	VariablesListWindow varWin(this, IedCon);
	varWin.exec();
	std::cout<<"Adding variables ret"<<std::endl;
	
}

void ExplorerWindow::onConnect()
{
	std::cout<<"Connection request"<<std::endl;
	IedClientError error;
	bool isConnected=false;
	switch(IedConnection_getState(IedCon))
	{
		case IED_STATE_CONNECTED:
			std::cout<<"Disconnecting"<<std::endl;
			IedConnection_close(IedCon);
			break;
		case IED_STATE_CLOSED:
		case IED_STATE_IDLE:
			std::cout<<"Trying to connect"<<std::endl;
			IedConnection_connect(IedCon, &error, lineEditServer->text().toStdString().c_str(), lineEditPort->text().toInt());
			if(error==IED_ERROR_OK)
				isConnected=true;
			else
			{
				QMessageBox::warning(this, tr("Connection error"), tr("Connection failed with error %1").arg(error));
				isConnected=false;
			}
			break;
	}
	if(isConnected)
	{
		addVar->setEnabled(true);
		connectBtn->setText("Disconnect");
	}
	else
	{
		addVar->setEnabled(false);
		connectBtn->setText("Connect");
	}
}

int main(int argc, char **argv)
{
	QApplication app (argc, argv);
	ExplorerWindow window;
	window.show();
	return app.exec();
}
