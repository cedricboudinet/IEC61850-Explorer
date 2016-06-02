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
#include "MmsValueWrapper.h"
#include "MmsTreeItem.h"
#include "ExplorerWindow.h"

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
	connect(this, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), SLOT(onItemDoubleClicked(QTreeWidgetItem*, int)));
}

void VariablesView::onItemDoubleClicked(QTreeWidgetItem *item, int column)
{
	if(column==1)
	{
		((MmsTreeItem*)(item))->onEdit((ExplorerWindow*)(parent()));
	}
}

void VariablesView::customMenuRequested(QPoint pos)
{

	QMenu *menu=new QMenu(this);
	QAction * delSel = new QAction("Delete", this);
	menu->addAction(delSel);
	QAction * selAction = menu->exec(viewport()->mapToGlobal(pos));
	if(selAction == delSel)
		deleteSelection();
}

void VariablesView::addVariables(const std::vector<MmsValueWrapper> & varList)
{
	for(std::vector<MmsValueWrapper>::const_iterator it=varList.begin(); it!=varList.end();it++)
	{
		new MmsTreeItem(this, *it);
	}

}

void VariablesView::refresh(IedConnection IedCon, const QString& server, int port)
{
	IedClientError error;
	setCursor(Qt::BusyCursor);
	IedConnection_connect(IedCon, &error, server.toStdString().c_str(), port);
	if(error == IED_ERROR_OK)
	{
		QTreeWidgetItemIterator it(this);
		while (*it)
		{
			((MmsTreeItem*)(*it))->update(IedCon);
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

void VariablesView::deleteSelection(void)
{
	foreach(QTreeWidgetItem * selItem, selectedItems())
		delete selItem;
}
