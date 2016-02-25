#ifndef QQUICKLOOKCAMERA_H
#define QQUICKLOOKCAMERA_H

#include <QtWidgets/QMainWindow>
#include "ui_qquicklookcamera.h"
#include "myclass.h"
#include <QInputDialog>
#include "instruct/Command.h"
#include <QMouseEvent>
#include <QComboBox>
#include<QPushButton>
#include<QDockWidget>
#include<qframe.h>
#include<qgroupbox.h>
#include <QFileDialog>
#include "../UI/ag_cg_dialog.h"
class QquickLookCamera : public QMainWindow
{
	Q_OBJECT

public:
	QquickLookCamera(QWidget *parent = 0);
	~QquickLookCamera();

	void createControlFrame();
	void createActions();
	void createMenus();
	// void createToolBars();
	void cerateStatus();

protected:
   
public slots :
	void showOpenFile();
    void saveFlie();
	void stopComs();
	void autoExposure();
	void manualExposure();
	void showAboutMSg();
	void showExpoTime();
	void showDigGain();
	void showATI();
	void showAG_CG();
	void showRegister();
	void rotate90();
private:   //主界面
	Ui::QquickLookCameraClass ui;
    QString digtalGain;  //全局变量保存上一次输入值
	QString eTime;
	QString filename;
	MyClass *showWidget;
	Command *command;
	QFrame *ctrlFrame;
	QDockWidget *ctrlFrameDock;

private:	//新建的dialog
	AG_CG_Dialog *ag_cg_dialog;
	QComboBox *AG_CGcomBox;

private:
	QLineEdit *expTimeEdit;
	QLineEdit *digtalGainEdit;
	QLineEdit *setATIEdit;
	QLineEdit *setAG_CG_Edit;
	QLineEdit *setRegisterDddr;
	QLineEdit *setRegisterDate;
private:
	QMenu *fileMenu;    //菜单栏
	QMenu *netParaMenu;
	QMenu *cameraContrMenu;
	QMenu *cameraParaMenu;
	QMenu *rotateMenu;
	QMenu *modeMenu;
	QMenu *helpMenu;

	QAction *openFileAction;
	QAction *saveAction;
	QAction *exitAction;
	QAction *setNetAction;
	QAction *setExpoTimeAction;
	QAction *setDigGainAction;
	QAction *setATIAction;
	QAction *setAG_CG_Action;
	QAction *setRegisterAction;
	QAction *rotate90Action;
	QAction *autoAction;
	QAction *manualAction;
	QAction *startAction;
	QAction *stopAction;
	QAction *mode1Action;
	QAction *mode2Action;
	QAction *aboutAction;

	QToolBar *fileTool;
	QToolBar *zoomTool;
	QToolBar *rotateTool;
	QToolBar *mirrorTool;
	QToolBar *doToolBar;

	QLabel  *statusLabel;  //状态栏
	QLabel  *framRate;
	
};

#endif // QQUICKLOOKCAMERA_H
