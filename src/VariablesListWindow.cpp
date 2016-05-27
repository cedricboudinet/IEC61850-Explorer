/// @author Cedric Boudinet
/// @license GNU GPL Version 3
///
/// Distributed under the GNU GPL version 3 License
/// (See accompanying file LICENSE or copy at
/// http://www.gnu.org/licenses/)
///
#include "VariablesListWindow.h"
#include "iec61850Exp_fun.h"
#include <QGridLayout>
#include <QDialogButtonBox>
#include <QListWidget>
#include <iostream>
#include "iec61850Exp_fun.h"
#include "MmsValueWrapper.h"
#include "VariablesView.h"
#include "MmsTreeItem.h"
VariablesListWindow::VariablesListWindow(QWidget *parent, IedConnection iedCon) : QDialog(parent), _iedCon(iedCon)
{
	QGridLayout *layout = new QGridLayout;
	setWindowTitle(tr("Variable list"));
	setModal(true);
	QDialogButtonBox * buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok| QDialogButtonBox::Cancel);
	connect(buttonBox, SIGNAL(accepted()), this, SLOT(onOK()));
	connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
	variableListWidget=new VariablesView(this);
	variableListWidget->setSelectionMode( QAbstractItemView::ExtendedSelection );
	variableListWidget->setColumnHidden(1, true);
	layout->addWidget(variableListWidget,1,0,1,1);
	layout->addWidget(buttonBox,2,0,1,1);
	setLayout(layout);
	resize(400, 300);
	_VariablesList = getVariableList(iedCon);
	variableListWidget->addVariables(_VariablesList);
}


void VariablesListWindow::onOK()
{
	accept();
}

std::vector<MmsValueWrapper> VariablesListWindow::getSelection()
{
	std::vector<MmsValueWrapper> selection;
	QTreeWidgetItemIterator it(variableListWidget);
	foreach(QTreeWidgetItem * it, variableListWidget->selectedItems())
	{
		selection.push_back(((MmsTreeItem*)(it))->getMmsValueWrapper());
		++it;
	}
	return selection;

}

