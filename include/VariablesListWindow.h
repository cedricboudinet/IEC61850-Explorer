/// @author Cedric Boudinet
/// @copyright GNU GPL Version 3
///
/// Distributed under the GNU GPL version 3 License
/// (See accompanying file LICENSE or copy at
/// http://www.gnu.org/licenses/)
///

#ifndef VARIABLESLISTWINDOW_H
#define VARIABLESLISTWINDOW_H
#include <QDialog>
#include <libiec61850/iec61850_client.h>

class QListWidget;
class MmsValueWrapper;
#include "MmsValueWrapper.h"
class VariablesView;

/** Dialog box displaying a list of Mms values on a server and allowing user to select some of them.*/
class VariablesListWindow : public QDialog
{
	Q_OBJECT
	public:
		/** Class constructor*/
		VariablesListWindow(QWidget *parent, IedConnection);
		/** Returns a list of selected Mms values */
		std::vector<MmsValueWrapper> getSelection();

	private:
		VariablesView *variableListWidget;
		IedConnection _iedCon;
		std::vector<MmsValueWrapper> _VariablesList;
		

	public slots:
		/** Called when user clicks on OK button.*/
		void onOK();
};

#endif
