/// @author Cedric Boudinet
/// @license GNU GPL Version 3
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
class VariablesListWindow : public QDialog
{
	Q_OBJECT
	public:
		VariablesListWindow(QWidget *parent, IedConnection);
		std::vector<MmsValueWrapper> getSelection();

	private:
		VariablesView *variableListWidget;
		IedConnection _iedCon;
		std::vector<MmsValueWrapper> _VariablesList;
		

	public slots:
		void onOK();
};

#endif
