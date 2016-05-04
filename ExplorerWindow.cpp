#include <QPushButton>
#include <QListView>
#include <QLabel>
#include <QGridLayout>
#include <QMessageBox>
#include <QHeaderView>
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

	iecVarTable = new QTableWidget(0,2);
	iecVarTable->setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred));
	QStringList labels;
	labels << tr("Variable") << tr("Value");
	iecVarTable->setHorizontalHeaderLabels(labels);
	iecVarTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
	iecVarTable->verticalHeader()->hide();

	QGridLayout *layout = new QGridLayout;
	layout->addWidget(labelServer, 0, 0);
	layout->addWidget(lineEditServer, 0, 1);
	layout->addWidget(labelPort, 1, 0);
	layout->addWidget(lineEditPort, 1, 1);
	//layout->addWidget(connectBtn, 2, 0, 1, 1);
	layout->addWidget(addVar, 2, 0, 1, 1);
	layout->addWidget(refreshBtn, 2, 1, 1, 1);
	layout->addWidget(iecVarTable, 3, 0, 1, 3);

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
			int row = iecVarTable->rowCount();
			for(QStringList::iterator it=varList.begin(); it!=varList.end();it++)
			{
				iecVarTable->insertRow(row);
				iecVarTable->setItem(row, 0, new QTableWidgetItem(*it));
				iecVarTable->setItem(row, 1, new QTableWidgetItem(""));
			}
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
	IedClientError error;
	IedConnection_connect(IedCon, &error, lineEditServer->text().toStdString().c_str(), lineEditPort->text().toInt());
	if(IedConnection_getState(IedCon)== IED_STATE_CONNECTED)
	{
		MmsValue * myMms;
		char buffer[100];
		for(int i=0;i<iecVarTable->rowCount();i++)
		{
			std::string varName = iecVarTable->item(i,0)->text().toStdString();
			std::string fc=varName.substr(varName.size()-3,2);
			varName=varName.substr(0,varName.size()-5);
			myMms = IedConnection_readObject(IedCon, &error, varName.c_str(), FunctionalConstraint_fromString(fc.c_str()));
			iecVarTable->item(i,1)->setText(MmsValue_printToBuffer(myMms, buffer, 100));
		}
		IedConnection_close(IedCon);
	}
}
