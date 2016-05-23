/// @author Cedric Boudinet
/// @license GNU GPL Version 3
///
/// Distributed under the GNU GPL version 3 License
/// (See accompanying file LICENSE or copy at
/// http://www.gnu.org/licenses/)
///
#include "MmsTreeItem.h"
MmsTreeItem::MmsTreeItem(QTreeWidget* parent, const QString & varName):QTreeWidgetItem(parent)
{
	setText(0, varName);
	setText(1, "");
	setFlags(flags() | Qt::ItemIsEditable);
	_varName = varName.toStdString();
	_fc=FunctionalConstraint_fromString(_varName.substr(_varName.size()-3,2).c_str());
	_varName=_varName.substr(0,varName.size()-5);
	
}

void MmsTreeItem::update(IedConnection IedCon)
{
	IedClientError error; //TODO : handle read error
	MmsValue * myMms;
	char buffer[100];
	myMms = IedConnection_readObject(IedCon, &error, _varName.c_str(), _fc);
	setText(1,MmsValue_printToBuffer(myMms, buffer, 100));
	MmsValue_delete(myMms);
	
}
