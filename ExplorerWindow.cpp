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
#include <QCheckBox>
#include <QLineEdit>
#include <QListView>
#include <QTableWidget>

#include "VariablesView.h"
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

	useAuth = new QCheckBox(tr("&Use authentication"));
	passwdLE = new QLineEdit("");
	passwdLE->setEchoMode(QLineEdit::Password);
	passwdLE->setEnabled(false);

	connect(useAuth, SIGNAL(clicked()), this, SLOT(onUseAuth()));
	connect(passwdLE, SIGNAL(editingFinished()), this, SLOT(updateAuth()));

	addVar = new QPushButton(tr("&Add variables"));
	connect(addVar, SIGNAL(clicked()), this, SLOT(onAddVar()));

	refreshBtn = new QPushButton(tr("&Refresh"));
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
	
	QHBoxLayout *authLayout = new QHBoxLayout;
	authLayout->addWidget(useAuth);
	authLayout->addWidget(passwdLE);
	layout->addLayout(authLayout);

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
	resize(800, 600);
}

void ExplorerWindow::onAddVar()
{
	IedClientError error;
	setCursor(Qt::BusyCursor);
	IedConnection_connect(IedCon, &error, lineEditServer->text().toStdString().c_str(), lineEditPort->text().toInt());
	setCursor(Qt::ArrowCursor);
	if(error==IED_ERROR_OK)
	{
		VariablesListWindow varWin(this, IedCon);
		if(varWin.exec())
		{
			QStringList varList = varWin.getSelection();
			iecVarTable->addVariables(varList);
		}
		IedConnection_close(IedCon);
	}
	else
	{
		QMessageBox::warning(this, tr("Connection error"), tr("Connection failed with error %1").arg(error));
	}
}

void ExplorerWindow::onRefresh()
{
	setCursor(Qt::BusyCursor);
	iecVarTable->refresh(IedCon, lineEditServer->text(), lineEditPort->text().toInt());
	setCursor(Qt::ArrowCursor);
}

void ExplorerWindow::onUseAuth()
{
	passwdLE->setEnabled(useAuth->isChecked());
	updateAuth();
}

void ExplorerWindow::updateAuth()
{
	static MmsConnection mmsConnection = IedConnection_getMmsConnection(IedCon);
	static IsoConnectionParameters parameters = MmsConnection_getIsoConnectionParameters(mmsConnection);
	static AcseAuthenticationParameter auth = (AcseAuthenticationParameter) calloc(1, sizeof(struct sAcseAuthenticationParameter));
	if(useAuth->isChecked())
	{
		char * passwd=strdup(passwdLE->text().toStdString().c_str());
		AcseAuthenticationParameter_setPassword(auth, passwd);
		AcseAuthenticationParameter_setAuthMechanism(auth, ACSE_AUTH_PASSWORD);
		IsoConnectionParameters_setAcseAuthenticationParameter(parameters, auth);
		free(passwd);
	}
	else
	{
		AcseAuthenticationParameter_setAuthMechanism(auth, ACSE_AUTH_NONE);
	}
}
