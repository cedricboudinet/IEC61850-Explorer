#ifndef VARIABLESLISTWINDOW_H
#define VARIABLESLISTWINDOW_H
#include <QWidget>
#include <QDialog>
#include <QPushButton>
#include <QListWidget>
#include <libiec61850/iec61850_client.h>

class VariablesListWindow : public QDialog
{
	Q_OBJECT
	public:
		VariablesListWindow(QWidget *parent, IedConnection);

	private:
		QListWidget *variableListWidget;

	public slots:
		void onOK();
};

#endif
