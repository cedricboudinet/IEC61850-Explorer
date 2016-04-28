#ifndef IEC61850EXPLORERGUI_H
#define IEC61850EXPLORERGUI_H
#include <libiec61850/iec61850_client.h>
#include <QWidget>
#include <QLineEdit>
#include <QTextBrowser>
#include <QPushButton>
#include <QListView>

class ExplorerWindow : public QWidget
{
	Q_OBJECT
	public:
		ExplorerWindow(QWidget *parent = 0);

	private:
		QLineEdit *lineEditServer;
		QLineEdit *lineEditPort;
		QTextBrowser * textViewer;
		QPushButton * addVar;
		QPushButton * connectBtn;
		IedConnection IedCon;

	public slots:
		void onConnect();
		void onAddVar();
};

#endif // IEC61850EXPLORERGUI_H
