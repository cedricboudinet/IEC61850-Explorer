/// @author Cedric Boudinet
/// @license GNU GPL Version 3
///
/// Distributed under the GNU GPL version 3 License
/// (See accompanying file LICENSE or copy at
/// http://www.gnu.org/licenses/)
///
#ifndef EXPLORERWINDOW_H
#define EXPLORERWINDOW_H
#include <libiec61850/iec61850_client.h>
#include <QWidget>

class VariablesView;
class QLineEdit;
class QCheckBox;
class QPushButton;
class QTimer;

class ExplorerWindow : public QWidget
{
	Q_OBJECT
	public:
		ExplorerWindow(QWidget *parent = 0);

	private:
		QLineEdit *lineEditServer;
		QLineEdit *lineEditPort;
		VariablesView *iecVarTable;
		QPushButton * addVar;
		QPushButton * connectBtn;
		QPushButton * refreshBtn;
		IedConnection IedCon;
		QCheckBox *useAuth;
		QLineEdit *passwdLE;
		QCheckBox *autorefreshChkBx;
		QLineEdit *autorefreshPeriod;
		QTimer *autorefreshTimer;

	public slots:
		void onAddVar();
		void onRefresh();
		void onAutorefreshConfig();
		void onUseAuth();
		void updateAuth();
};

#endif // IEC61850EXPLORERGUI_H
