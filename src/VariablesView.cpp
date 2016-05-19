/// @author Cedric Boudinet
/// @license GNU GPL Version 3
///
/// Distributed under the GNU GPL version 3 License
/// (See accompanying file LICENSE or copy at
/// http://www.gnu.org/licenses/)
///
#include "VariablesView.h"
#include <QMenu>
#include <QHeaderView>
#include <QMessageBox>
#include <iostream>

VariablesView::VariablesView(QWidget *parent) : QTreeWidget(parent)
{
	setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred));
	QStringList labels;
	labels << tr("Variable") << tr("Value");
	setHeaderLabels(labels);
	header()->setSectionResizeMode(0, QHeaderView::Stretch);
	header()->setSectionResizeMode(1, QHeaderView::Stretch);
	setSelectionMode( QAbstractItemView::ExtendedSelection );

	setEditTriggers(QAbstractItemView::NoEditTriggers);
	setSelectionBehavior(QAbstractItemView::SelectRows);
	setContextMenuPolicy(Qt::CustomContextMenu);
	setRootIsDecorated(false);
	connect(this, SIGNAL(customContextMenuRequested(QPoint)), SLOT(customMenuRequested(QPoint)));
}

void VariablesView::customMenuRequested(QPoint pos)
{

	QMenu *menu=new QMenu(this);
	menu->addAction(new QAction("Delete", this));
	QAction * selAction = menu->exec(viewport()->mapToGlobal(pos));
	if(selAction)
	{
		foreach(QTreeWidgetItem * selItem, selectedItems())//TODO
			delete selItem;
	}
}

void VariablesView::addVariables(const QStringList& varList)
{
	QTreeWidgetItem *after = 0;
	QTreeWidgetItem *item;
	for(QStringList::const_iterator it=varList.begin(); it!=varList.end();it++)
	{
		item = new QTreeWidgetItem(this, after);
		item->setText(0, (*it));
		item->setText(1, "");
		item->setFlags(item->flags() | Qt::ItemIsEditable);
	}

}

void VariablesView::refresh(IedConnection IedCon, const QString& server, int port)
{
	IedClientError error;
	setCursor(Qt::BusyCursor);
	IedConnection_connect(IedCon, &error, server.toStdString().c_str(), port);
	if(error == IED_ERROR_OK)
	{
		MmsValue * myMms;
		char buffer[100];
		QTreeWidgetItemIterator it(this);
		while (*it)
		{
			std::string varName = (*it)->text(0).toStdString();
			std::string fc=varName.substr(varName.size()-3,2);
			varName=varName.substr(0,varName.size()-5);
			myMms = IedConnection_readObject(IedCon, &error, varName.c_str(), FunctionalConstraint_fromString(fc.c_str()));
			(*it)->setText(1,MmsValue_printToBuffer(myMms, buffer, 100));
			++it;
		}
		IedConnection_close(IedCon);
		setCursor(Qt::ArrowCursor);
	}
	else
	{
		setCursor(Qt::ArrowCursor);
		QMessageBox::warning(this, tr("Connection error"), tr("Connection failed with error %1").arg(error));
	}

}

