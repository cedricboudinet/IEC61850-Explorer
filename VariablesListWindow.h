#ifndef VARIABLESLISTWINDOW_H
#define VARIABLESLISTWINDOW_H
#include <QDialog>
#include <libiec61850/iec61850_client.h>

class QListWidget;

class VariablesListWindow : public QDialog
{
	Q_OBJECT
	public:
		VariablesListWindow(QWidget *parent, IedConnection);
		QStringList getSelection();

	private:
		QListWidget *variableListWidget;
		IedConnection _iedCon;
		std::map<std::string, FunctionalConstraint> _VariablesList;
		

	public slots:
		void onOK();
};

#endif
