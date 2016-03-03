/********************************************************************************
** Form generated from reading UI file 'qquicklookcamera.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QQUICKLOOKCAMERA_H
#define UI_QQUICKLOOKCAMERA_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QquickLookCameraClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *QquickLookCameraClass)
    {
        if (QquickLookCameraClass->objectName().isEmpty())
            QquickLookCameraClass->setObjectName(QStringLiteral("QquickLookCameraClass"));
        QquickLookCameraClass->resize(600, 400);
        menuBar = new QMenuBar(QquickLookCameraClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        QquickLookCameraClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(QquickLookCameraClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        QquickLookCameraClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(QquickLookCameraClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        QquickLookCameraClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(QquickLookCameraClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        QquickLookCameraClass->setStatusBar(statusBar);

        retranslateUi(QquickLookCameraClass);

        QMetaObject::connectSlotsByName(QquickLookCameraClass);
    } // setupUi

    void retranslateUi(QMainWindow *QquickLookCameraClass)
    {
        QquickLookCameraClass->setWindowTitle(QApplication::translate("QquickLookCameraClass", "QquickLookCamera", 0));
    } // retranslateUi

};

namespace Ui {
    class QquickLookCameraClass: public Ui_QquickLookCameraClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QQUICKLOOKCAMERA_H
