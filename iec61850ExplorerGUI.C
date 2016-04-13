#include <QApplication>
#include <QPushButton>
#include <QTableWidget>

int main(int argc, char **argv)
{
	QApplication app (argc, argv);
	QTableWidget * table = new QTableWidget();
	table->setRowCount(10);
	table->setColumnCount(3);
	table->show();

	return app.exec();
}
