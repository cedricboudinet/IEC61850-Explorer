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
	QInputDialog *newValDialog = new QInputDialog();
	bool ok;
	switch(_myMms.getType())
	{
		case MMS_FLOAT :
			{
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
			}
			break;
		case MMS_VISIBLE_STRING:
			{
				QString oldVal(text(1));
				QString newVal = newValDialog->getText(explWin, "Change value", "New value:", QLineEdit::Normal, text(1), &ok);
				if(ok)
				{
					if(explWin->setIedConnectionState(true))
					{
						_myMms.setStringValue(explWin->getIedConnection(), newVal.toStdString());
						update(explWin->getIedConnection());
						explWin->setIedConnectionState(false);
					}
				}
			}
			break;
		case MMS_INTEGER:
			{
				int oldVal = QLocale::system().toInt(text(1));
				int result = newValDialog->getInt(explWin, "Change value", "New value:", oldVal, -1e9, 1e9, 1, &ok);
				if(ok)
				{
					if(explWin->setIedConnectionState(true))
					{
						_myMms.setIntegerValue(explWin->getIedConnection(), result);
						update(explWin->getIedConnection());
						explWin->setIedConnectionState(false);
					}
				}
			}
			break;
		case MMS_BOOLEAN:
		default://TODO
			std::cout << "Data type modification not handled :"<<_myMms.getType()<<std::endl;
			break;
			
	}
	delete newValDialog;
}

