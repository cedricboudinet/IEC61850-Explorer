#include "VariablesListWindow.h"
#include "iec61850Exp_fun.h"
#include <QGridLayout>
#include <QDialogButtonBox>
#include <iostream>
VariablesListWindow::VariablesListWindow(QWidget *parent, IedConnection iedCon) : QDialog(parent)
{
	QGridLayout *layout = new QGridLayout;
	setWindowTitle(tr("Variable list"));
	setModal(true);
	QDialogButtonBox * buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok| QDialogButtonBox::Cancel);
	connect(buttonBox, SIGNAL(accepted()), this, SLOT(onOK()));
	connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
	variableListWidget=new QListWidget(this);
	variableListWidget->setSelectionMode( QAbstractItemView::ExtendedSelection );
	layout->addWidget(variableListWidget,1,0,1,1);
	layout->addWidget(buttonBox,2,0,1,1);
	setLayout(layout);
	std::map<std::string, FunctionalConstraint> VariablesList = getVariableList(iedCon);
	for(std::map<std::string, FunctionalConstraint>::iterator it =VariablesList.begin();it!=VariablesList.end();it++)
		variableListWidget->addItem(QString(it->first.c_str()));
}


void VariablesListWindow::onOK()
{
	std::cout<<"TODO : add variables"<<std::endl;
	accept();
}

