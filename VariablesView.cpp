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
VariablesView::VariablesView(QWidget *parent) : QTableWidget(0,2, parent)
{
	setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred));
	horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
	QStringList labels;
	labels << tr("Variable") << tr("Value");
	setHorizontalHeaderLabels(labels);
	horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
	verticalHeader()->hide();
	setEditTriggers(QAbstractItemView::NoEditTriggers);
	setSelectionBehavior(QAbstractItemView::SelectRows);
	setContextMenuPolicy(Qt::CustomContextMenu);
	connect(this, SIGNAL(customContextMenuRequested(QPoint)), SLOT(customMenuRequested(QPoint)));
}

void VariablesView::customMenuRequested(QPoint pos)
{
	//QModelIndex index=indexAt(pos);

	QMenu *menu=new QMenu(this);
	menu->addAction(new QAction("Delete", this));
	QAction * selAction = menu->exec(viewport()->mapToGlobal(pos));
	if(selAction)
	{
		int i=0;
		while(i<rowCount())
		{
			if(item(i,0)->isSelected())
				removeRow(i);
			else
				i++;
		}
	}
}

void VariablesView::addVariables(const QStringList& varList)
{
	int row = rowCount();
	for(QStringList::const_iterator it=varList.begin(); it!=varList.end();it++)
	{
		insertRow(row);
		setItem(row, 0, new QTableWidgetItem(*it));
		setItem(row, 1, new QTableWidgetItem(""));
	}

}

void VariablesView::refresh(IedConnection IedCon, const QString& server, int port)
{
	IedClientError error;
	IedConnection_connect(IedCon, &error, server.toStdString().c_str(), port);
	if(IedConnection_getState(IedCon)== IED_STATE_CONNECTED)
	{
		MmsValue * myMms;
		char buffer[100];
		for(int i=0;i<rowCount();i++)
		{
			std::string varName = item(i,0)->text().toStdString();
			std::string fc=varName.substr(varName.size()-3,2);
			varName=varName.substr(0,varName.size()-5);
			myMms = IedConnection_readObject(IedCon, &error, varName.c_str(), FunctionalConstraint_fromString(fc.c_str()));
			item(i,1)->setText(MmsValue_printToBuffer(myMms, buffer, 100));
		}
		IedConnection_close(IedCon);
	}

}

