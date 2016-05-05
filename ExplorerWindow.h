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
#include <QLineEdit>
#include <QPushButton>
#include <QListView>
#include <QTableWidget>
#include "VariablesView.h"

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

	public slots:
		void onConnect();
		void onAddVar();
		void onRefresh();
};

#endif // IEC61850EXPLORERGUI_H
