#ifndef IEC61850EXPLORERGUI_H
#define IEC61850EXPLORERGUI_H

#include <QWidget>
#include <QLineEdit>
#include <QTextBrowser>

class ExplorerWindow : public QWidget
{
	Q_OBJECT
	public:
		ExplorerWindow(QWidget *parent = 0);

	private:
		QLineEdit *lineEditServer;
		QLineEdit *lineEditPort;
		QTextBrowser * textViewer;

	public slots:
		void onBrowse();
};





#endif // IEC61850EXPLORERGUI_H
