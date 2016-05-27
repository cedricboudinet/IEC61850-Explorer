/// @author Cedric Boudinet
/// @license GNU GPL Version 3
///
/// Distributed under the GNU GPL version 3 License
/// (See accompanying file LICENSE or copy at
/// http://www.gnu.org/licenses/)
///

#ifndef VARIABLESVIEW_H
#define VARIABLESVIEW_H
#include <QTreeWidget>
#include <libiec61850/iec61850_client.h>
class VariablesView : public QTreeWidget
{
	Q_OBJECT
	public:
		VariablesView(QWidget *parent = 0);
		void addVariables(const QStringList&);
		void refresh(IedConnection, const QString& server, int port);
	public slots:
		void customMenuRequested(QPoint);
		void deleteSelection(void);

};

#endif // VARIABLESVIEW_H
