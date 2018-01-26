#include "qquicklookcamera.h"
#include <QMessageBox>
#include <QInputDialog>
#include <QMouseEvent>
#include <QComboBox>
#include <QPushButton>
#include <QDockWidget>
#include <qframe.h>
#include <qgroupbox.h>
#include <QFileDialog>
#include <QLabel>
#include <QBoxLayout>

QquickLookCamera::QquickLookCamera(QWidget *parent)
	: QMainWindow(parent)
{
	//ui.setupUi(this);
	setWindowTitle(tr("CMOS Camera System Software"));
	setWindowIcon(QIcon("realtime.png"));

	imageLayout = new QGridLayout;
	imageWidget = new QWidget;

	showWidget0 = new MyClass(InstructionUnit::CMOSE, this);
	showWidget1 = new MyClass(InstructionUnit::CMOS1, this);
	showWidget2 = new MyClass(InstructionUnit::CMOS2, this);
	showWidget3 = new MyClass(InstructionUnit::CMOS3, this);
	imageLayout->addWidget(showWidget0,0,0);
	imageLayout->addWidget(showWidget1,0,1);
	imageLayout->addWidget(showWidget2,1,0);
	imageLayout->addWidget(showWidget3,1,1);
	imageWidget->setLayout(imageLayout);
	setCentralWidget(imageWidget);
	
	//ctrlFrame = new QFrame;
	//createControlFrame(); 
	//ctrlFrameDock = new QDockWidget(tr("Fuction Zone"), this);
	//ctrlFrameDock->setFixedWidth(180);
	//ctrlFrameDock->setFeatures(QDockWidget::AllDockWidgetFeatures);
	//ctrlFrameDock->setWidget(ctrlFrame);
	//addDockWidget(Qt::LeftDockWidgetArea, ctrlFrameDock);

	//cerateStatus();    //创建状态栏
}

QquickLookCamera::~QquickLookCamera()
{

}
//void QquickLookCamera::createControlFrame()
//{
//	//曝光时间
//	QLabel *expoTimeLabel = new QLabel(tr("exposure time:"));
//	expoTimeLineEdit = new QLineEdit;
//	expoTimeLineEdit->setPlaceholderText(tr("0000"));
//	connect(expoTimeLineEdit, &QLineEdit::textChanged, this, [this]()
//	{
//		bool ok;
//		QString valueStr = expoTimeLineEdit->text();
//		expoTime = valueStr.toInt(&ok);
//	});
//	QPushButton *expoTimeOKButton = new QPushButton(tr("OK"));
//	connect(expoTimeOKButton, &QPushButton::clicked, this, [this]()
//	{
//		setExpoTime(expoTime);
//	});
//	//帧率
//	QLabel *frRateLabel = new QLabel(tr("frame rate:")); 
//	frRateLineEdit = new QLineEdit;
//	frRateLineEdit->setPlaceholderText(tr("0000"));
//	connect(frRateLineEdit, &QLineEdit::textChanged, this, [this]()
//	{
//		bool ok;
//		QString valueStr = frRateLineEdit->text();
//		frRate = valueStr.toInt(&ok);
//	});
//	QPushButton *frRateOKButton = new QPushButton(tr("OK"));
//	connect(frRateOKButton, &QPushButton::clicked, this, [this]()
//	{
//		setFrRate(frRate);
//	});
//	//DG
//	QLabel *DGLabel = new QLabel(tr("digtal gain:"));
//	DGLineEdit = new QLineEdit;
//	DGLineEdit->setPlaceholderText(tr("00.00"));
//	connect(DGLineEdit, &QLineEdit::textChanged, this, [this]()
//	{
//		bool ok;
//		QString valueStr = frRateLineEdit->text();
//		dg = valueStr.toFloat(&ok);
//	});
//	QPushButton *DGOKButton = new QPushButton(tr("OK"));
//	connect(DGOKButton, &QPushButton::clicked, this, [this]()
//	{
//		setDG(dg);
//	});
//	//AG&CG
//	QLabel *AGCGLabel = new QLabel(tr("Gain Total(AG&CG):"));
//	AGCGComboBox = new QComboBox;
//	AGCGComboBox->addItem(tr("1.00"));
//	AGCGComboBox->addItem(tr("1.14"));
//	AGCGComboBox->addItem(tr("1.33"));
//	AGCGComboBox->addItem(tr("1.60"));
//	AGCGComboBox->addItem(tr("2.00"));
//	AGCGComboBox->addItem(tr("2.29"));
//	AGCGComboBox->addItem(tr("2.67"));
//	AGCGComboBox->addItem(tr("3.20"));
//	AGCGComboBox->addItem(tr("4.00"));
//	AGCGComboBox->addItem(tr("5.33"));
//	AGCGComboBox->addItem(tr("8.00"));
//	connect(AGCGComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(AGCGBoxChanged()));
//	
//	QPushButton *AGCGOKButton = new QPushButton(tr("OK"));
//	connect(AGCGOKButton, &QPushButton::clicked, this, [this]()
//	{
//		setAGCG(ag_cg);
//	});
//
//	QGridLayout *cameraParaLayout = new QGridLayout;
//	//cameraParaLayout->setContentsMargins(8, 8, 8, 8);      //设置布局内部四边的空隙
//	//cameraParaLayout->setAlignment(Qt::AlignHCenter);
//	////cameraParaLayout->setAlignment(Qt::AlignTop);
//	//cameraParaLayout->setSpacing(15);                      //设置间距
//	//cameraParaLayout->setMargin(4);
//
//	cameraParaLayout->addWidget(expoTimeLabel, 0, 0);
//	cameraParaLayout->addWidget(expoTimeLineEdit, 1, 0);
//	cameraParaLayout->addWidget(expoTimeOKButton, 2, 0);
//
//	cameraParaLayout->addWidget(frRateLabel, 3, 0);
//	cameraParaLayout->addWidget(frRateLineEdit, 4, 0);
//	cameraParaLayout->addWidget(frRateOKButton, 5, 0);
//
//	cameraParaLayout->addWidget(DGLabel, 6, 0);
//	cameraParaLayout->addWidget(DGLineEdit, 7, 0);
//	cameraParaLayout->addWidget(DGOKButton, 8, 0);
//
//	cameraParaLayout->addWidget(AGCGLabel, 9, 0);
//	cameraParaLayout->addWidget(AGCGComboBox, 10, 0);
//	cameraParaLayout->addWidget(AGCGOKButton, 11, 0);
//
//	QGroupBox *paraBox = new QGroupBox(tr("parameter settings"));
//	paraBox->setContentsMargins(20, 20, 20, 20);
//	paraBox->setLayout(cameraParaLayout);
//
//	QPushButton *startButton = new QPushButton(tr("StartUpload"));
//	connect(startButton, &QPushButton::clicked, this, &QquickLookCamera::AECRun);
//	QPushButton *stopButton = new QPushButton(tr("StopUpload"));
//	connect(stopButton, &QPushButton::clicked, this, &QquickLookCamera::Stop);
//	QPushButton *saveButton = new QPushButton(tr("SavePicture"));
//	connect(saveButton, &QPushButton::clicked, this, &QquickLookCamera::saveFlie);
//
//	//Frame布局
//	QGridLayout *frameLayout = new QGridLayout;
//	frameLayout->setMargin(5);
//	frameLayout->setSpacing(10);
//	frameLayout->addWidget(startButton, 0, 0);
//	frameLayout->addWidget(stopButton, 1, 0);
//	frameLayout->addWidget(saveButton, 2, 0);
//	frameLayout->addWidget(paraBox, 3, 0);
//	ctrlFrame->setLayout(frameLayout);
//}
//
//void QquickLookCamera::cerateStatus()
//{
//	infoLabel = new QLabel;
//	infoLabel->setText(tr(" | Tips: "));
//	infoLabel->setFixedWidth(300);
//	//statusBar()->addPermanentWidget(infoLabel);
//
//	frRateLabel = new QLabel;
//	QString tempfr = tr(" | frame rate: ");
//	tempfr += QString::number(frRate);
//	tempfr += tr(" fps");
//	frRateLabel->setText(tempfr);
//	frRateLabel->setFixedWidth(150);
//	//statusBar()->addPermanentWidget(frRateLabel);
//
//	frLengthLabel = new QLabel;
//	QString tempfl = tr(" | frame length: ");
//	tempfl += QString::number(frLength);
//	frLengthLabel->setText(tempfl);
//	frLengthLabel->setFixedWidth(150);
//	//statusBar()->addPermanentWidget(frLengthLabel);
//
//	expoTimeLabel = new QLabel;
//	QString tempet = tr(" | exposure time: ");
//	tempet += QString::number(expoTime);
//	expoTimeLabel->setText(tempet);
//	expoTimeLabel->setFixedWidth(150);
//	//statusBar()->addPermanentWidget(expoTimeLabel);
//}
//
//void QquickLookCamera::OpenFile()
//{
//	//qDebug()<< "------------qqucickdianjichengggong---------------";
//	filename = QFileDialog::getOpenFileName(this);
//}
//void QquickLookCamera::saveFlie()
//{
//	//showWidget->setSaveFile();
//}
//
//void QquickLookCamera::AECRun()
//{
//	//InstructionProxy *instruct = new InstructionProxy(Instruction::CMOS1);
//	//instruct->AECRun();
//	//delete instruct;
//	InstructionProxy *instruct1 = new InstructionProxy(Instruction::CMOSE);
//	instruct1->AECRun();
//	delete instruct1;
//	//InstructionProxy *instruct2 = new InstructionProxy(Instruction::CMOS2);
//	//instruct2->AECRun();
//	//delete instruct2;
//	//InstructionProxy *instruct3 = new InstructionProxy(Instruction::CMOS3);
//	//instruct3->AECRun();
//	//delete instruct3;
//	uploadFlag = true;
//	infoLabel->setText(tr(" | Tips: Data is alreay upload!"));
//	infoLabel->update();
//}
//void QquickLookCamera::Stop()
//{
//	InstructionProxy instruct(Instruction::CMOSE);
//	for (int i = 0; i < 10; ++i)
//	{
//		instruct.Stop();
//	}
//
//	uploadFlag = false;
//	infoLabel->setText(tr(" | Tips: Data stop uploading!"));
//	infoLabel->update();
//}
//
//void QquickLookCamera::setExpoTime(unsigned int _time)
//{
//	if (!uploadFlag)
//	{
//		QMessageBox::critical(this, tr("Error"), tr("Data has not been upload! Please click the dataupload button!"));
//		return;
//	}
//	if (expoTime >= frLength)
//	{
//		expoTime = frLength;
//		QMessageBox::critical(this, tr("Error"), tr("Exposure time cannot be bigger than frame length!"));
//	}
//	else
//		expoTime = _time;
//
//	InstructionProxy *instruct = new InstructionProxy(Instruction::CMOSE);
//	//instruct.SetDG(dg);
//	instruct->setExpoTime(expoTime);
//	delete instruct;
//	//instruct.SetAGCG(ag_cg);
//
//	QString tempet = tr(" | exposure time: ");
//	tempet += QString::number(expoTime);
//	expoTimeLabel->setText(tempet);
//	expoTimeLabel->update();
//	
//	uploadFlag = true;
//	infoLabel->setText(tr(" | Tips: Data is already upload!"));
//	infoLabel->update();
//}
//void QquickLookCamera::setFrRate(int _rate)
//{
//	if (!uploadFlag)
//	{
//		QMessageBox::critical(this, tr("Error"), tr("Data has not been upload! Please click the dataupload button!"));
//		return;
//	}
//	frRate = _rate;
//	frLength = 180000 / frRate;
//
//	InstructionProxy instruct(Instruction::CMOSE);
//	instruct.SetFPS(frRate);
//
//	QString tempfr = tr(" | frame rate: ");
//	tempfr += QString::number(frRate);
//	tempfr += tr(" fps");
//	frRateLabel->setText(tempfr);
//	frRateLabel->update();
//
//	QString tempfl = tr(" | frame length: ");
//	tempfl += QString::number(frLength);
//	frLengthLabel->setText(tempfl);
//	frLengthLabel->update();
//
//	uploadFlag = true;
//	infoLabel->setText(tr(" | Tips: Data is alreay upload!"));
//	infoLabel->update();
//}
//void QquickLookCamera::setAGCG(float _totalGain)
//{
//	if (!uploadFlag)
//	{
//		QMessageBox::critical(this, tr("Error"), tr("Data has not been upload! Please click the dataupload button!"));
//		return;
//	}
//	ag_cg = _totalGain;
//
//	InstructionProxy instruct(Instruction::CMOSE);
//	//instruct.SetDG(dg);
//	//instruct.setExpoTime(expoTime);
//	instruct.SetAGCG(ag_cg);
//
//	uploadFlag = true;
//	infoLabel->setText(tr(" | Tips: Data is alreay upload!"));
//	infoLabel->update();
//}
//void QquickLookCamera::setDG(float _dg)
//{
//	if (!uploadFlag)
//	{
//		QMessageBox::critical(this, tr("Error"), tr("Data has not been upload! Please click the dataupload button!"));
//		return;
//	}
//	dg = _dg;
//
//	InstructionProxy instruct(Instruction::CMOSE);
//	instruct.SetDG(dg);
//	//instruct.setExpoTime(expoTime);
//	//instruct.SetAGCG(ag_cg);
//
//	uploadFlag = true;
//	infoLabel->setText(tr(" | Tips: Data is alreay upload!"));
//	infoLabel->update();
//}
//
//void QquickLookCamera::AGCGBoxChanged()
//{
//	bool ok;
//	QString valueStr = AGCGComboBox->currentText();
//	ag_cg = valueStr.toFloat(&ok);
//}