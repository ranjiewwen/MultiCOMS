#include "qquicklookcamera.h"
#include <QtWidgets/QApplication>
#include <QTranslator>
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
    QTranslator *translator=new QTranslator;
    translator->load("C:/Users/CMOS/Desktop/QquickLookCamera/QquickLookCamera/QquickLookCamera.qm");
    a.installTranslator(translator);
	QquickLookCamera w;
	w.setGeometry(180,50,950,600);
	w.show();
	return a.exec();
}
