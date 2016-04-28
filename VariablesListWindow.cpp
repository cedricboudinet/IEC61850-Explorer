#include "VariablesListWindow.h"
#include <QGridLayout>
#include <QDialogButtonBox>
#include <iostream>
VariablesListWindow::VariablesListWindow(QWidget *parent, IedConnection iedCon) : QDialog(parent)
{
	QGridLayout *layout = new QGridLayout;
	QPushButton * OKbtn = new QPushButton(tr("OK"));
	setWindowTitle(tr("Variable list"));
	std::cout<<"Dialog open"<<std::endl;
	setModal(true);
	QDialogButtonBox * buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok| QDialogButtonBox::Cancel);
	connect(buttonBox, SIGNAL(accepted()), this, SLOT(onOK()));
	connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
	layout->addWidget(buttonBox,2,0,2,1);
	setLayout(layout);
}


void VariablesListWindow::onOK()
{
	std::cout<<"TODO : add variables"<<std::endl;
	accept();
}

