#include "qquicklookcamera.h"
#include "QDebug"
#include <QMessageBox>
#include "../UI/ag_cg_dialog.h"
#include "../instruct/Command.h"
QquickLookCamera::QquickLookCamera(QWidget *parent)
: QMainWindow(parent),
command(new Command(4567, inet_addr("192.168.1.1"), 3955, inet_addr("192.168.1.2")))
{
	ui.setupUi(this);
	setWindowTitle(tr("CMOS"));
	setWindowIcon(QIcon("realtime.png"));
	//setGeometry(50,50,900,600);
	//QWidget *centralWidget = new QWidget;

	showWidget = new MyClass(this);
	showWidget->setMinimumSize(400,400);
	setCentralWidget(showWidget);  //设置主窗口界面
	ctrlFrame = new QFrame;
	createControlFrame();          //新建主窗口左侧的控制面板区
	ctrlFrameDock = new QDockWidget(tr("ctrlFrameDock"), this);
	ctrlFrameDock->setFixedWidth(180);
	ctrlFrameDock->setFeatures(QDockWidget::AllDockWidgetFeatures);
	ctrlFrameDock->setWidget(ctrlFrame);
	addDockWidget(Qt::LeftDockWidgetArea,ctrlFrameDock);

	//QGridLayout *mainLayout = new QGridLayout(centralWidget);//必须指明对中心窗口布局
	//mainLayout->setMargin(10);
	//mainLayout->setSpacing(20);
	//mainLayout->addWidget(ctrlFrame, 0, 1, 1, 3);
	//mainLayout->addWidget(showWidget, 0, 0, 1, 1);   //这样的布局，myclass不随主窗口而变化
	
	createActions();   //创建动作
	createMenus();     //创建菜单
	cerateStatus();    //创建状态栏
}

QquickLookCamera::~QquickLookCamera()
{

}
void QquickLookCamera::createControlFrame()//
{
	//相机控制
	QPushButton *autoExp = new QPushButton(tr("autoExp button"));
	connect(autoExp, &QPushButton::clicked , this, [this](){ command->AutoRun(); });   //注意用法
	QPushButton *manualExp = new QPushButton(tr("manulExp button"));
	connect(manualExp, &QPushButton::clicked, this, [this](){ command->ManulRun(); });
	QVBoxLayout *cameraCtrlLayout = new QVBoxLayout;
	cameraCtrlLayout->addWidget(autoExp);
	cameraCtrlLayout->addWidget(manualExp);
	QGroupBox  *cameraContrl = new QGroupBox(tr("cameraContrl"));
	cameraContrl->setContentsMargins(20, 20, 20, 20);
	cameraContrl->setLayout(cameraCtrlLayout);

	//相机参数
	QLabel *expTime = new QLabel(tr("exposure time")); //et（0-2EE0）-------16进制输入
	expTimeEdit = new QLineEdit;
	expTimeEdit->setPlaceholderText(tr("input hex(0-2EE0)"));
	connect(expTimeEdit, &QLineEdit::textChanged, this, [this]()
	{
		bool ok;
		QString valueStr = expTimeEdit->text();  
		int valueInt = valueStr.toInt(&ok);
		command->setET(valueInt);
	});
	QLabel *digtalGain = new QLabel(tr("digtalGain"));  //dg为小数，整数部分为5位，小数部分为7位
	digtalGainEdit = new QLineEdit;
	digtalGainEdit->setPlaceholderText(tr("setDigtalGain(5.7)(0-31.99):"));
	connect(digtalGainEdit, &QLineEdit::textChanged, this, [this]()
	{
		bool ok;
		QString valueStr = digtalGainEdit->text();
		int valueDouble = valueStr.toDouble(&ok);
		command->setDG(valueDouble);
	});
	QLabel *setATI = new QLabel(tr("set Target Illumination")); //tai（0-1023）默认值为0xb8-------10进制输入 
	setATIEdit = new QLineEdit;
	setATIEdit->setPlaceholderText(tr("input 0-1023(default0xb8):"));
	connect(setATIEdit, &QLineEdit::textChanged, this, [this]() 
	{ 
		bool ok;
		QString valueStr = setATIEdit->text();
		int valueInt = valueStr.toInt(&ok);
		command->setTAI(valueInt);
	});
	QLabel *setAG_CG = new QLabel(tr("set AFE&Column Gain"));   //gain为0-10中的任意数，表示增益倍数
	setAG_CG_Edit = new QLineEdit;
	setAG_CG_Edit->setPlaceholderText(tr("input 0-10:"));
	connect(setAG_CG_Edit, &QLineEdit::textChanged, this, [this]()
	{
		bool ok;
		QString valueStr = digtalGainEdit->text();
		int valueInt= valueStr.toInt(&ok);
		command->setDG(valueInt);
	});
	QLabel *setRegister = new QLabel(tr("write register"));   //addr：寄存器地址，9bit;  data：16bit
	QHBoxLayout *registerDddrLayout = new QHBoxLayout;
	QLabel *addressLabel = new QLabel(tr("address"));
	setRegisterDddr = new QLineEdit;
	setRegisterDddr->setPlaceholderText(tr("input address 9bit:"));
	registerDddrLayout->addWidget(addressLabel);
	registerDddrLayout->addWidget(setRegisterDddr);

	QHBoxLayout *registerDataLayout = new QHBoxLayout;
	QLabel *dataLabel = new QLabel(tr("data"));
	dataLabel->setAlignment(Qt::AlignCenter);
	//dataLabel->setFixedWidth(60);     //  layout->addStretch();/*   这里表示弹簧*/
	setRegisterDate = new QLineEdit;
	setRegisterDate->setPlaceholderText(tr("input data 16bit:"));
	registerDataLayout->addWidget(dataLabel);
	registerDataLayout->addWidget(setRegisterDate);

	QPushButton *okButton = new QPushButton(tr("make sure"));  
	connect(okButton, &QPushButton::clicked, this, [this]()
	{
		bool ok;
		QString valueStrAddr = setRegisterDddr->text();
		int valueIntAddr = valueStrAddr.toInt(&ok);	
		QString valueStrData = setRegisterDate->text();
		int valueIntData = valueStrData.toInt(&ok);
		command->writeRegister(valueIntAddr, valueIntData);
	});

	QGridLayout *cameraParaLayout = new QGridLayout;
	cameraParaLayout->setContentsMargins(8, 8, 8,8);//设置布局内部四边的空隙
	cameraParaLayout->setAlignment(Qt::AlignHCenter);
	//cameraParaLayout->setAlignment(Qt::AlignTop);
	cameraParaLayout->setSpacing(15);//设置间距
	cameraParaLayout->setMargin(4);
	
	cameraParaLayout->addWidget(expTime,0,0);
	cameraParaLayout->addWidget(expTimeEdit,1,0);
	cameraParaLayout->addWidget(digtalGain,2,0);
	cameraParaLayout->addWidget(digtalGainEdit,3,0);
	cameraParaLayout->addWidget(setATI,4,0);
	cameraParaLayout->addWidget(setATIEdit,5,0);
	cameraParaLayout->addWidget(setAG_CG,6,0);
	cameraParaLayout->addWidget(setAG_CG_Edit,7,0);
	cameraParaLayout->addWidget(setRegister,8,0);
	cameraParaLayout->addLayout(registerDddrLayout,9,0);
	cameraParaLayout->addLayout(registerDataLayout,10,0);
	cameraParaLayout->addWidget(okButton,11,0);
	QGroupBox *cameraPara = new QGroupBox(tr("camear parameter"));
	cameraPara->setContentsMargins(20,20,20,20);
	cameraPara->setLayout(cameraParaLayout);

	//Frame布局
	QGridLayout *frameLayout = new QGridLayout;
	frameLayout->setMargin(5);
	frameLayout->setSpacing(10);
	frameLayout->addWidget(cameraContrl,0,0,1,1);
	frameLayout->addWidget(cameraPara,1,0,4,1);
	ctrlFrame->setLayout(frameLayout);
}

void QquickLookCamera::createActions()
{
	
	// open action
	openFileAction = new QAction(QIcon("open.png"), tr("open"), this);
	openFileAction->setShortcut(tr("Ctrl+O"));
	openFileAction->setStatusTip(tr("open files"));
	connect(openFileAction, SIGNAL(triggered()), this, SLOT(showOpenFile()));
	//save action
	saveAction = new QAction(tr("save"), this);
	saveAction->setShortcut(tr("Ctrl+S"));
	saveAction->setStatusTip(tr("save current 4 image"));
	connect(saveAction, SIGNAL(triggered()), this, SLOT(saveFlie()));//点击存储ImageMagnifierItem里面的图像
	//exit action
	exitAction = new QAction(tr("exit"), this);
	exitAction->setShortcut(tr("Ctrl+Q"));
	exitAction->setStatusTip(tr("quit programmer"));
	connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));
	//setNetPa action
	setNetAction = new QAction(tr("set Net Parameter"), this);
	//set exposure time
	setExpoTimeAction = new QAction(tr("set exposure time"), this);
	setExpoTimeAction->setStatusTip("set ET");
	connect(setExpoTimeAction, SIGNAL(triggered()), this, SLOT(showExpoTime()));
	//set digital gain
	setDigGainAction = new QAction(tr("set digtal gain"), this);
	setDigGainAction->setStatusTip("set DG");
	connect(setDigGainAction, SIGNAL(triggered()), this, SLOT(showDigGain()));
	//set ATI action
	setATIAction = new QAction(tr("set ATI"), this);
	connect(setATIAction, SIGNAL(triggered()), this, SLOT(showATI()));
	//set SG_CG action
	setAG_CG_Action = new QAction(tr("set AG_CG"), this);
	connect(setAG_CG_Action, SIGNAL(triggered()), this, SLOT(showAG_CG()));
	//set register
	setRegisterAction = new QAction(tr("set register"), this);
	connect(setRegisterAction, SIGNAL(triggered()), this, SLOT(showRegister()));
	//set auto run 
	autoAction = new QAction(tr("autoRun"), this);
	connect(autoAction, SIGNAL(triggered()), this, SLOT(autoExposure()));
	//set manual run
	manualAction = new QAction(tr("maualRun"), this);
	connect(manualAction, SIGNAL(triggered()), this, SLOT(manualExposure()));
	//set rotate 
	rotate90Action = new QAction(tr("ratate90"),this);
	connect(rotate90Action,SIGNAL(triggered()),this,SLOT(rotate90()));
	//set stop 传输
	stopAction = new QAction(tr("stopRun"), this);
	connect(stopAction, SIGNAL(triggered()), this, SLOT(stopComs()));
	//set mode
	mode1Action = new QAction(tr("mode1"), this);
	mode2Action = new QAction(tr("mode2"), this);
	//about action
	aboutAction = new QAction(tr("about"), this);
	connect(aboutAction, SIGNAL(triggered()), this, SLOT(showAboutMSg()));
}

void QquickLookCamera::createMenus()
{

	fileMenu = menuBar()->addMenu(tr("file"));
	fileMenu->addAction(openFileAction);
	fileMenu->addAction(saveAction);
	fileMenu->addAction(exitAction);

	netParaMenu = menuBar()->addMenu(tr("netPara"));
	netParaMenu->addAction(setNetAction);

	cameraParaMenu = menuBar()->addMenu(tr("camePara"));
	cameraParaMenu->addAction(setExpoTimeAction);
	cameraParaMenu->addAction(setDigGainAction);
	cameraParaMenu->addAction(setATIAction);
	cameraParaMenu->addAction(setAG_CG_Action);
	cameraParaMenu->addAction(setRegisterAction);

	cameraContrMenu = menuBar()->addMenu(tr("cameContr"));
	cameraContrMenu->addAction(autoAction);
	cameraContrMenu->addAction(manualAction);
	cameraContrMenu->addAction(stopAction);

	rotateMenu = menuBar()->addMenu(tr("rotate image"));
	rotateMenu->addAction(rotate90Action);

	modeMenu = menuBar()->addMenu(tr("modeChoose"));
	modeMenu->addAction(mode1Action);
	modeMenu->addAction(mode2Action);

	helpMenu = menuBar()->addMenu(tr("help"));
	helpMenu->addAction(aboutAction);
}

void QquickLookCamera::cerateStatus()
{
	statusLabel = new QLabel;
	statusLabel->setText(tr("frame rate:"));
	statusLabel->setFixedWidth(50);
	framRate = new QLabel;
	framRate->setText(tr("0  0  0  0 "));
	framRate->setFixedWidth(100);
	statusBar()->addPermanentWidget(statusLabel);
	statusBar()->addPermanentWidget(framRate);
}

void QquickLookCamera::showOpenFile()
{
	//qDebug()<< "------------qqucickdianjichengggong---------------";
	filename = QFileDialog::getOpenFileName(this);
}
void QquickLookCamera::saveFlie()
{
	showWidget->setSaveFile();
}
void QquickLookCamera::rotate90()
{
	showWidget->setRotate90();
}
void QquickLookCamera::autoExposure()
{
	statusBar()->showMessage("-----start ----");
	command->AutoRun();   //connect直接写不行,用lamda表达式
}
void QquickLookCamera::manualExposure()
{
	command->ManulRun();
}
void QquickLookCamera::stopComs()
{
	statusBar()->showMessage("-----stop ----");
	command->stopCMOS();
}
void QquickLookCamera::showAboutMSg()
{
	QMessageBox::about(this, tr("About message box "), tr("this is about message box!"));
	return;
}

void QquickLookCamera::showExpoTime()
{
	/*bool ok;
	int eTime = QInputDialog::getInt(this, tr("input Hex:"),
	tr("Please setExposuerTime:"), expTimeLabel->text().toInt(&ok), 0, 100, 1, &ok);
	if (ok)
	command->setET(eTime);*/
	QLabel  *expTimeLabel = new QLabel;
	expTimeLabel->setText(eTime);   //曝光时间的初始值
	expTimeLabel->setFrameStyle(QFrame::Panel | QFrame::Sunken);
	bool ok;
	eTime = QInputDialog::getText(this, tr("input dialog:"), tr("input eTime"),
		QLineEdit::Normal, expTimeLabel->text(), &ok);
	bool ok1;
	int hex = eTime.toInt(&ok1, 16);
	while (hex < 0 || hex > 0x2ee0)
	{
		QMessageBox::critical(this, tr("Critical Message!"), tr("Please Input Again"));
		QString eTime = QInputDialog::getText(this, tr("input dialog:"), tr("input eTime"), //ET：0C00 （0000-2EE0）
			QLineEdit::Normal, expTimeLabel->text(), &ok);
		hex = eTime.toInt(&ok1, 16);
	}
	command->setET(hex);
}
void QquickLookCamera::showDigGain()
{
	QLabel  *digGainLabel = new QLabel;
	digGainLabel->setLineWidth(20);
	digGainLabel->setText(digtalGain);
	digGainLabel->setFrameStyle(QFrame::Panel | QFrame::Sunken);
	bool ok;
	digtalGain = QInputDialog::getText(this, tr("input double:"), tr("Please setDigtalGain(5.7)(0-31.99):"), //
		QLineEdit::Normal, digGainLabel->text(), &ok);
	double val = digtalGain.toDouble(); //val
	if (ok && !digtalGain.isEmpty())
		command->setDG(val);
}

void QquickLookCamera::showATI()
{
	command->setTAI();
}
void QquickLookCamera::showAG_CG()
{
	//digGainDialog = new QWidget(this);  //直接新建一个widget貌似不行，这里新建一个Dialog类
	ag_cg_dialog = new AG_CG_Dialog(this);
	ag_cg_dialog->setWindowFlags(Qt::Window);
	ag_cg_dialog->setWindowTitle("setAG_CG");
	ag_cg_dialog->show();
	//QString str = combox;
	//command->setAG_CG(/*ag_cg_dialog->getNumber()*/4);
}
void QquickLookCamera::showRegister()
{
	//command->writeRegister(addr, data);
}