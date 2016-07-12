/// @author Cedric Boudinet
/// @copyright GNU GPL Version 3
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

/*! class for IEC61850 explorer window */
class ExplorerWindow : public QWidget
{
	Q_OBJECT
	public:
		/** ExplorerWindow class constructor.*/
		ExplorerWindow(QWidget *parent = 0);
		/** Returns the IedConnection.*/
		IedConnection getIedConnection();
		/** (Dis)connects to the IED server.*/
		bool setIedConnectionState(bool);

	private:
		QLineEdit *lineEditServer;
		QLineEdit *lineEditPort;
		VariablesView *iecVarTable;
		QPushButton * addVar;
		QPushButton * connectBtn;
		QPushButton * refreshBtn;
		IedConnection _iedCon;
		QCheckBox *useAuth;
		QLineEdit *passwdLE;
		QCheckBox *autorefreshChkBx;
		QLineEdit *autorefreshPeriod;
		QTimer *autorefreshTimer;

	public slots:
		/** Called when user clicks on 'Add variable'.\n Shows the variable list window and adds variables if result is true */
		void onAddVar();
		/** Called when user clicks on 'Refresh' or periodically when autoRefresh is enabled.\n Updates the displayed Mms variables.*/
		void onRefresh();
		/** Called when user changes the refresh period or changes the autorefresh checkbox.\n Configures the refresh timer.*/
		void onAutorefreshConfig();
		/** Called when user clicks on 'Use authentication' checkbox.\n Updates the authentication parameters.*/
		void onUseAuth();
		/** Called when user changes authentication password.\n Updates authentication parameters.*/
		void updateAuth();
};

#endif // IEC61850EXPLORERGUI_H
