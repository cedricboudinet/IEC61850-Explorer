/// @author Cedric Boudinet
/// @copyright GNU GPL Version 3
///
/// Distributed under the GNU GPL version 3 License
/// (See accompanying file LICENSE or copy at
/// http://www.gnu.org/licenses/)
///

#ifndef VARIABLESVIEW_H
#define VARIABLESVIEW_H
#include <QTreeWidget>
#include <libiec61850/iec61850_client.h>
class MmsValueWrapper;

/*! Widget displaying a list of variables and their values.*/
class VariablesView : public QTreeWidget
{
	Q_OBJECT
	public:
		/** VariablesView constructor */
		VariablesView(QWidget *parent = 0);
		/** Adds a list of Mms values to the tree
		\param varList list of Mms values to be added*/
		void addVariables(const std::vector<MmsValueWrapper> & varList);
		/** Updates the mms values
		\param IedCon connection to IED
		\param server IED server name
		\param port IED server port
		*/
		void refresh(IedConnection, const QString& server, int port);
	public slots:
		/** Called when context menu is requested */
		void customMenuRequested(QPoint);
		/** Called when user asks for Mms items deletion */
		void deleteSelection();
		/** Called when user double-clicks on an item
		\param item the item the user doubled-clicked on
		\param column column index on which the user clicked*/
		void onItemDoubleClicked(QTreeWidgetItem *item, int column);
};

#endif // VARIABLESVIEW_H
