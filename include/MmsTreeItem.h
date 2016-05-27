/// @author Cedric Boudinet
/// @license GNU GPL Version 3
///
/// Distributed under the GNU GPL version 3 License
/// (See accompanying file LICENSE or copy at
/// http://www.gnu.org/licenses/)
///
#ifndef MMSTREEITEM_H
#define MMSTREEITEM_H
#include <QTreeWidgetItem>
#include <libiec61850/iec61850_client.h>
#include <string>
#include <libiec61850/iec61850_client.h>
#include "MmsValueWrapper.h"

class MmsTreeItem : public QTreeWidgetItem
{
	public:
		MmsTreeItem(QTreeWidget *parent, MmsValueWrapper);
		void update(IedConnection);
		MmsValueWrapper getMmsValueWrapper();

	private:
		MmsValueWrapper _myMms;

};

#endif // VARIABLESVIEW_H
