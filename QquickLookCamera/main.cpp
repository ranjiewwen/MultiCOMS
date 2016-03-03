#include "qquicklookcamera.h"
#include <QtWidgets/QApplication>
#include <QTranslator>
int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
    QTranslator *translator=new QTranslator;
    translator->load("../QquickLookCamera/QquickLookCamera.qm");
    app.installTranslator(translator);
	QquickLookCamera w;
	w.setGeometry(180,50,950,600);
	w.show();
	return app.exec();
}
