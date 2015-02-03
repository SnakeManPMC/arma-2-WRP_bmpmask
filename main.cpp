#include <QtWidgets/QApplication>
#include "widget.h"
#include <QDebug>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	Widget w;

	if ( argc < 2 )
	{
		qDebug() << "Not enough arguments!";
	}
	else
	{
		qDebug() << "Running the tool now...";
		w.ReadFile(argv[1]);
	}

	return 0;
}
