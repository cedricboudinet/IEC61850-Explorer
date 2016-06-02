/// @author Cedric Boudinet
/// @license GNU GPL Version 3
///
/// Distributed under the GNU GPL version 3 License
/// (See accompanying file LICENSE or copy at
/// http://www.gnu.org/licenses/)
///
#include "MmsTreeItem.h"
#include <QInputDialog>
#include <iostream>
MmsTreeItem::MmsTreeItem(QTreeWidget* parent, MmsValueWrapper theMms):
QTreeWidgetItem(parent), _myMms(theMms)
{
	setText(0, _myMms.getName().c_str());
	setText(1, "");
	setFlags(flags() | Qt::ItemIsEditable);
	
}

void MmsTreeItem::update(IedConnection IedCon)
{
	setText(1,_myMms.getValueAsString(IedCon).c_str());
}

MmsValueWrapper MmsTreeItem::getMmsValueWrapper()
{
	return _myMms;
}

void MmsTreeItem::onEdit(ExplorerWindow * explWin)
{
	if(_myMms.getType() == MMS_FLOAT)
	{
		QInputDialog *newValDialog = new QInputDialog();
		bool ok;
		double oldVal = QLocale::system().toDouble(text(1));
		double result = newValDialog->getDouble(explWin, "Change value", "New value:", oldVal, -1e9, 1e9, 6, &ok);
		if(ok)
		{
			if(explWin->setIedConnectionState(true))
			{
				_myMms.setFloatValue(explWin->getIedConnection(), result);
				update(explWin->getIedConnection());
				explWin->setIedConnectionState(false);
			}
		}
		delete newValDialog;
	}
}

