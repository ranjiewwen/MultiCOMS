#include "myclass.h"
#include "Instruction\InstructionUnit.h"
#include <iostream>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "qquicklookcamera.h"
class qquickLookCamera;
MyClass::MyClass(InstructionUnit::CMOSID _cmosId, QWidget *parent, Qt::WindowFlags f)
	: QWidget(parent, f),
	cmosId(_cmosId),
	m_height(1024), 
	m_imgWidth(1280),
	m_assWidth(20), 
	m_angle(0),
	m_BufPicNum(64)
{
	if (_cmosId == InstructionUnit::CMOSE)
	{
		m_inputSrc = new InputCMOS(4008, inet_addr("192.168.1.2"));
	}
	else if (_cmosId == InstructionUnit::CMOS1)
	{
		m_inputSrc = new InputCMOS(4004, inet_addr("192.168.1.2"));
	}
	else if (_cmosId == InstructionUnit::CMOS2)
	{
		m_inputSrc = new InputCMOS(4002, inet_addr("192.168.1.2"));
	}
	else if (_cmosId == InstructionUnit::CMOS3)
	{
		m_inputSrc = new InputCMOS(4001, inet_addr("192.168.1.2"));
	}
	//m_rotatedDataProvider = new RotatedImageDataItem(m_angle);
	//初始化数据缓冲区
	std::dynamic_pointer_cast<CCirQueue>(m_cmosData)->Initial(m_height * (m_assWidth + m_imgWidth) * m_BufPicNum, (m_assWidth + m_imgWidth));//每次允许弹出一行数据
	std::dynamic_pointer_cast<CCirQueue>(m_cmosImageData)->Initial(m_height * m_imgWidth * m_BufPicNum, m_height * m_imgWidth);//每次允许弹出一张图片的数据
	//注册缓冲区
	m_inputSrc->registerOutputBuffer(0, m_cmosData); 
	std::dynamic_pointer_cast<ImageDataItem>(m_dataProvider)->registerInputBuffer(0, m_cmosData);
	std::dynamic_pointer_cast<ImageDataItem>(m_dataProvider)->registerOutputBuffer(0, m_cmosImageData);
	std::dynamic_pointer_cast<RotatedImageDataItem>(m_rotatedDataProvider)->registerInputBuffer(0, m_cmosImageData);

	//开始上传数据
	m_inputSrc->start();
	//开始图像解析
	m_dataProvider->setup(m_assWidth, m_imgWidth, m_height);
	//开始图像旋转
	m_rotatedDataProvider->setup(m_assWidth, m_imgWidth, m_height);
	const FeaturesOfDataItem* rotatedFeatures = m_rotatedDataProvider->constDataFeatures();

    //窗口绑定数据指针，初始化m_dataProvider
	m_window->setDataItemPtr(m_rotatedDataProvider);
	//窗口绑定数据转换指针，初始化m_convertor
	m_window->setConvertor(std::shared_ptr<PixelConvertor>(new Pixel8To32));
	//设置保存文件，初始化m_file
	m_window->setSave(std::shared_ptr<SaveToFile>(new SaveToBmpGray));


	m_magnifier->setDataItemPtr(m_rotatedDataProvider);
	//窗口绑定数据转换指针，初始化m_convertor
	m_magnifier->setConvertor(std::shared_ptr<PixelConvertor>(new Pixel8To32));
	//设置保存文件，初始化m_file
	m_magnifier->setSave(std::shared_ptr<SaveToFile>(new SaveToBmpGray));

	//设置窗口大小
	m_window->setMagnifierRange(rotatedFeatures->payloadDataWidth, rotatedFeatures->linesPerFrame);
	//设置放大的窗口大小
	m_magnifier->setMagnifierRange(rotatedFeatures->payloadDataWidth, rotatedFeatures->linesPerFrame);
	//定时刷新界面
	m_refreshTimer = new QTimer(this);	
	connect(m_refreshTimer, &QTimer::timeout, this, [this](){m_window->refresh(); });//捕获列表中捕获了它所在函数中的局部变量，才能在函数体中使用该变量。
	connect(m_refreshTimer, &QTimer::timeout, this, [this](){m_magnifier->refresh(); });
	m_refreshTimer->start(30); //每隔30ms更新一次
	
	//m_magnifier->show();
	m_magnifier->setCmosNumber(cmosId);
	m_window->setCmosNumber(cmosId);
	connect(m_window, &ImageWindowItem::cursorPositionChanged, m_magnifier, &ImageMagnifierItem::onCursorPositionChanged);
	connect(m_window, &ImageWindowItem::mouseDoubleClicked, this, &MyClass::onMouseDoubleClicked);
	connect(m_window, &ImageWindowItem::rotateActionTriggerd, this, &MyClass::rotateImage);

	//默认界面
	gridLayout->addWidget(m_window);
	setLayout(gridLayout);
}

MyClass::~MyClass()
{
}

void MyClass::rotateImage()
{
	if (m_angle == 270)
		m_angle = 0;
	else
		m_angle += 90;
	std::dynamic_pointer_cast<RotatedImageDataItem>(m_rotatedDataProvider)->changeAngle(m_angle);
	const FeaturesOfDataItem* rotatedFeatures = m_rotatedDataProvider->constDataFeatures();
	m_window->setMagnifierRange(rotatedFeatures->payloadDataWidth, rotatedFeatures->linesPerFrame);
	m_magnifier->setMagnifierRange(rotatedFeatures->payloadDataWidth, rotatedFeatures->linesPerFrame);
}