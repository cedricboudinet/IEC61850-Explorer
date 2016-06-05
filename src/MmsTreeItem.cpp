/// @author Cedric Boudinet
/// @license GNU GPL Version 3
///
/// Distributed under the GNU GPL version 3 License
/// (See accompanying file LICENSE or copy at
/// http://www.gnu.org/licenses/)
///
#include "MmsTreeItem.h"
#include "MmsInputDialog.h"
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
	MmsInputDialog *newValDialog = new MmsInputDialog();
	bool ok;
	MmsValue * newVal = newValDialog->getMmsValue(_myMms, ok); //text(1)
	if(ok)
	{
		if(explWin->setIedConnectionState(true))
		{
			_myMms.setMmsValue(explWin->getIedConnection(), newVal);
			update(explWin->getIedConnection());
			explWin->setIedConnectionState(false);
		}
	}
	MmsValue_delete(newVal);
	delete newValDialog;
}

