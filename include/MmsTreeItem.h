/// @author Cedric Boudinet
/// @copyright GNU GPL Version 3
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
#include "MmsValueWrapper.h"
#include "ExplorerWindow.h"

/*! class for displaying Mms variables and value in a QTreeWidget */
class MmsTreeItem : public QTreeWidgetItem
{
	public:
		/** MmsTreeItem constructor */
		MmsTreeItem(QTreeWidget *parent, MmsValueWrapper);
		/** Updates the Mms value */
		void update(IedConnection);
		/** Returns a MmsValueWrapper linked to the Mms variable */
		MmsValueWrapper getMmsValueWrapper();
		/** When called, displays a input dialog asking for a new value, then updates the value in the server */
		void onEdit(ExplorerWindow*);

	private:
		MmsValueWrapper _myMms;

};

#endif // VARIABLESVIEW_H
