#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

namespace Ui
{
	class Widget;
}

class Widget : public QWidget {
	Q_OBJECT
public:
	Widget(QWidget *parent = 0);
	~Widget();

	void ReadFile(char *wrpname);
	void CreateWRP_to_BMP(char *filename);

	// 32 chars in array of 512.
	char TexStrings[512][32];

	// texture indexes
	short TexIndex;

	// wrp size
	int MapSize;

protected:
	void changeEvent(QEvent *e);

private:
	Ui::Widget *ui;
};

#endif // WIDGET_H
