#include "gui.h"
#include <QtGui/QApplication>
#include <QTextCodec>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	gui w;
	w.show();
	QTextCodec *tc=QTextCodec::codecForName("CP1251");
    QTextCodec::setCodecForCStrings(tc);
	w.setWindowTitle("Игра \"Жизнь\"");
	return a.exec();
}
