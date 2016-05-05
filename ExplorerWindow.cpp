/// @author Cedric Boudinet
/// @license GNU GPL Version 3
///
/// Distributed under the GNU GPL version 3 License
/// (See accompanying file LICENSE or copy at
/// http://www.gnu.org/licenses/)
///
#include <QPushButton>
#include <QListView>
#include <QLabel>
#include <QGridLayout>
#include <QMessageBox>
#include <QTimer>

#include "ExplorerWindow.h"
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

	//connectBtn = new QPushButton(tr("&Connect"));
	//connect(connectBtn, SIGNAL(clicked()), this, SLOT(onConnect()));

	addVar = new QPushButton(tr("&Add variables"));
	//addVar->setEnabled(false);
	connect(addVar, SIGNAL(clicked()), this, SLOT(onAddVar()));

	refreshBtn = new QPushButton(tr("&Refresh"));
	//refreshBtn->setEnabled(false);
	connect(refreshBtn, SIGNAL(clicked()), this, SLOT(onRefresh()));

	iecVarTable = new VariablesView(this);

	QVBoxLayout *layout = new QVBoxLayout;
	QHBoxLayout *serverLayout = new QHBoxLayout;
	serverLayout->addWidget(labelServer);
	serverLayout->addWidget(lineEditServer);
	layout->addLayout(serverLayout);

	QHBoxLayout *portLayout = new QHBoxLayout;
	portLayout->addWidget(labelPort);
	portLayout->addWidget(lineEditPort);
	layout->addLayout(portLayout);
	//layout->addWidget(connectBtn, 2, 0, 1, 1);

	QHBoxLayout *btnsLayout = new QHBoxLayout;
	btnsLayout->addWidget(addVar);
	btnsLayout->addWidget(refreshBtn);
	btnsLayout->addStretch();
	layout->addLayout(btnsLayout);
	layout->addWidget(iecVarTable);

	setLayout(layout);
	std::string title = "IEC61850 Explorer v ";
	title+=IECEXP_VERSION;
	setWindowTitle(title.c_str());
	IedCon = IedConnection_create();
	/*QTimer *timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(onRefresh()));
	timer->start(100);*/
	resize(800, 600);
}

void ExplorerWindow::onAddVar()
{
	std::cout<<"Adding variables"<<std::endl;
	IedClientError error;
	IedConnection_connect(IedCon, &error, lineEditServer->text().toStdString().c_str(), lineEditPort->text().toInt());
	if(IedConnection_getState(IedCon)== IED_STATE_CONNECTED)
	{
		VariablesListWindow varWin(this, IedCon);
		if(varWin.exec())
		{
			QStringList varList = varWin.getSelection();
			iecVarTable->addVariables(varList);
		}
		IedConnection_close(IedCon);
	}
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
		refreshBtn->setEnabled(true);
		connectBtn->setText("Disconnect");
	}
	else
	{
		addVar->setEnabled(false);
		refreshBtn->setEnabled(false);
		connectBtn->setText("Connect");
	}
}

void ExplorerWindow::onRefresh()
{
	iecVarTable->refresh(IedCon, lineEditServer->text(), lineEditPort->text().toInt());
}

