#ifndef QQUICKLOOKCAMERA_H
#define QQUICKLOOKCAMERA_H

#include <QMainWindow>
#include "myclass.h"
#include <QCloseEvent>
#include "../Instruction/InstructionProxy.h"
class QLabel;
class QComboBox;
class QLineEdit;
class QFrame;

//该类提供整个上位机的主窗口
class QquickLookCamera : public QMainWindow
{
	Q_OBJECT
public:
	QquickLookCamera(QWidget *parent = 0);
	~QquickLookCamera();
	void closeEvent(QCloseEvent *ev)
	{
		if (!showWidget0->onMainWindowClosed() || !showWidget1->onMainWindowClosed() || !showWidget2->onMainWindowClosed() || !showWidget3->onMainWindowClosed())
		{//stop failed
			QMessageBox::StandardButton button = QMessageBox::question(this, tr("exit program"), tr("waring: Data is still uploading!Still to exit the program?"), QMessageBox::No | QMessageBox::Yes,QMessageBox::No);
			if (button == QMessageBox::No||button==QMessageBox::Cancel)
				ev->ignore();
			else if (button == QMessageBox::Yes)
				ev->accept();
		}
	}
	//创建相机控制子窗口
	//void createControlFrame();
	//创建状态栏
	//void cerateStatus();

//public slots :
	////void OpenFile();
	//void saveFlie();
	//void AECRun();
	//void Stop();
	//void setExpoTime(unsigned int _time);
	//void setFrRate(int _rate);
	//void setAGCG(float _totalGain);
	//void setDG(float _dg);
	//void AGCGBoxChanged();

private: 
//	//Ui::QquickLookCameraClass ui; //不使用UI，使用纯代码的方式添加控件
//
//	QString filename;
	QWidget *imageWidget;
	QGridLayout *imageLayout;
	MyClass *showWidget0;
	MyClass *showWidget1;
	MyClass *showWidget2;
	MyClass *showWidget3;
//	QFrame *ctrlFrame;
//	QDockWidget *ctrlFrameDock;
//
//private:
//	QLineEdit *expoTimeLineEdit;
//	QLineEdit *frRateLineEdit;
//	QLineEdit *DGLineEdit;
//	//QLineEdit *AGCGLineEdit;
//	QComboBox *AGCGComboBox;
//
//	unsigned int expoTime{ 2000 };
//	int frRate{ 18 };
//	unsigned int frLength{ 10000 };
//	float dg{1.0};
//	float ag_cg{1.00};
//	bool uploadFlag{ false };//数据上传标志
};

#endif // QQUICKLOOKCAMERA_H
