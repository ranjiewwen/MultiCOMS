#include "ArrayCameraDataItem.h"
#include "IBuffer.h"
#include "FeaturesOfDataItem.h"

#include <string.h>  //memcpy

#include "initsock.h"

static const unsigned short framehead = 0xeb91;

//构造函数：辅助数据
ArrayCameraDataItem::ArrayCameraDataItem()
{
    m_features->attachedDataSize = POINT_SIZE::SIZE_1;
    m_features->payloadDataSize = POINT_SIZE::SIZE_1;
}

ArrayCameraDataItem::~ArrayCameraDataItem()
{

}

//设置长宽高
bool ArrayCameraDataItem::setup(int assWidth, int imgWidth, int height)
{
    if (assWidth <0 || imgWidth <= 0 || height <= 0)
        return false;
    std::shared_ptr<unsigned char> img[2], ass[2];
    for (int i=0; i<2; ++i)
    {
        img[i].reset(new unsigned char [imgWidth*height*static_cast<int>(m_features->payloadDataSize)]);
        if (!img[i])
            return false;
		ass[i].reset(new unsigned char[assWidth*height*static_cast<int>(m_features->attachedDataSize)]);
		if (!ass[i])
			return false;
    }

    //stop processing , reset image&assist data ptr, set features of width&height
	IDataProcessUnit::stop();
    //need lock ?
    for (int i=0; i<2; ++i)
    {
        m_dualAssistBuffer[i] = ass[i];
        m_dualImageBuffer[i] = img[i];
    }

    //testing  初始化缓冲区1中的数据
    unsigned char* buf = &(*img[1]);
    unsigned char* abuf = &(*ass[1]);
    for (int h=0; h<height; ++h)
    {
        for (int w=0;w<imgWidth; ++w)
        {
            buf[h*imgWidth+w] = w;
        }
        for (int w=0; w<assWidth; ++w)
        {
            abuf[h*assWidth+w] = h;
        }
    }
    //end testing

    //start processing
	IDataItem::setup(assWidth, imgWidth, height);
	IDataProcessUnit::start();

	return true;
}

void ArrayCameraDataItem::process()
{
    resetCounter();
    m_curFrameCnt = 0;

    unsigned char* buf = nullptr;
	int headerSize = sizeof(CMOS_FRAME_HEAD);
    int imgLineSize = m_features->payloadDataWidth*static_cast<int>(m_features->payloadDataSize);
    int assLineSize = m_features->attachedDataWidth*static_cast<int>(m_features->attachedDataSize);
    int lineSize = imgLineSize+assLineSize;
    
    std::shared_ptr<IBuffer>& inputBuffer_zero = m_inputBuffer[0].second;  //从缓冲区内取数据，然后去帧头
	CMOS_FRAME_HEAD* pkgHeader = nullptr;
    while (m_processing)
    {
		
		if (!inputBuffer_zero)//若inputBuffer没有被注册
        {
			//printf("\nErr:inputBuffer没有被注册");
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            continue;
        }
		if (0 != inputBuffer_zero->front(buf, headerSize))//将包头放入缓冲区buf
        {
			//printf("\nErr:将包头放入缓冲区buf");
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            continue;
        }
		
		pkgHeader = reinterpret_cast<CMOS_FRAME_HEAD*>(buf);
		if (framehead != htons(pkgHeader->frmhead))//检验帧头
		{
			inputBuffer_zero->pop_front(1);
			//printf("\nErr:检验帧头=%X", pkgHeader->frmhead);
			continue;
		}	
		//将整个数据包放入缓冲区buf
		if (0 != inputBuffer_zero->front(buf, imgLineSize + headerSize))//1280为有效数据长度
        {
			//printf("\nErr:将整个数据包放入缓冲区buf");
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            continue;
        }
		
		storePayloadData(buf);    //存储数据（采用双缓冲形式），包括丢帧处理
		inputBuffer_zero->pop_front(imgLineSize + headerSize);//将该数据包弹出
    }
}  

void ArrayCameraDataItem::storePayloadData(const unsigned char *buf)
{
	//面阵相机包格式处理流程：
	//提取帧计数（从0开始），检查行号，帧计数跳变后，行号应该从0开始，帧计数未跳变时，根据分通道检查图像数据
	//行号是否+1递增，正常则填入数据缓存相应的位置，错误则报错。未
	//满一帧但行号归0，报错，行标识不对，报错，包长度不对，报错。

	const CMOS_FRAME_HEAD* pkgHeader = reinterpret_cast<const CMOS_FRAME_HEAD*>(buf);//指向包头
	int headerSize = sizeof(CMOS_FRAME_HEAD);
	int frmCnt = htons(pkgHeader->frmCount);//帧计数
	int curLineCnt = htons(pkgHeader->lineCount);//行号

	if (-1 == m_newFrame)//第一次
	{
		//printf("\nfirst time!!");
		if (curLineCnt != 0)
		{
			//printf("\nhalf-baked frame appears!!");
			return;
		}
		m_newFrame = frmCnt; //set current framecnt then next
		if (frmCnt != m_curFrameCnt + 1)
		{

			//printf("\nframe counter error!!");
			//return;
		}
		m_curFrameCnt = frmCnt;
	}

	int& lineCnt = m_curLineCnt;
	int imgLineSize = m_features->payloadDataWidth*static_cast<int>(m_features->payloadDataSize);
	int assLineSize = m_features->attachedDataWidth*static_cast<int>(m_features->attachedDataSize);
	int lineSize = imgLineSize + assLineSize;

	//帧计数未跳变，行号加1递增
	if (frmCnt == m_newFrame && (lineCnt + 1 == curLineCnt || -1 == lineCnt))
	{
		lineCnt = curLineCnt;
		if (curLineCnt >= m_features->linesPerFrame)
		{
			//printf("\n>=(1024) lines");
			return;
		}
		if (m_dualImageBuffer[0])
		{
			unsigned char *buffer = m_dualImageBuffer[0].get() + curLineCnt*imgLineSize;
			for (int i = 0; i < imgLineSize; ++i)
			{
				buffer[i] = buf[imgLineSize - i + headerSize];  //buf->m_dualImageBuffer[0]
			}
		}

		if (curLineCnt == m_features->linesPerFrame - 1)//一帧的最后一行，将缓冲区[0]放入[1]中
		{
			resetCounter();  //reset and look for the first line of next img
			if (m_dualImageBuffer[0] && m_dualImageBuffer[1])
				memcpy(m_dualImageBuffer[1].get(), m_dualImageBuffer[0].get(), m_features->linesPerFrame*imgLineSize);

			//数据放到输出缓冲区
			if (m_outputBuffer[0].second)
				memcpy(m_outputBuffer[0].second.get(), m_dualImageBuffer[1].get(), m_features->linesPerFrame*imgLineSize);
		}
	}
	else
	{
		resetCounter();  //reset and look for the first line of next img
	}
}

//copy area of image data
bool ArrayCameraDataItem::copyArea(int x, int y, int w, int h, unsigned char* extPtr) const
{
	if (!m_dualImageBuffer[1] || !extPtr)
		return false;

	unsigned char* buffer = m_dualImageBuffer[1].get();
	for (int _h = 0; _h < h; ++_h) {
		memcpy(extPtr + _h*w*static_cast<int>(m_features->payloadDataSize),
			&(*buffer) + ((y + _h)*m_features->payloadDataWidth + x) * static_cast<int>(m_features->payloadDataSize),
			w*static_cast<int>(m_features->payloadDataSize));
	}

	return true;
}

//get entire image data ptr
bool ArrayCameraDataItem::internalImageDataPtr(const unsigned char*& ptr) const
{
	if (!m_dualImageBuffer[1] || !ptr)
		return false;
	ptr = m_dualImageBuffer[1].get();
	return true;
}

//get entire assist data ptr
bool ArrayCameraDataItem::internalAssistDataPtr(const unsigned char*& ptr) const
{
	ptr = m_dualAssistBuffer[1].get();
	return true;
}


