#include "ImageDataItem.h"
#include "IBuffer.h"
#include "FeaturesOfDataItem.h"

#include <string.h>  //memcpy

#include "initsock.h"

//构造函数：辅助数据
ImageDataItem::ImageDataItem()
{
    //m_features->attachedDataSize = POINT_SIZE::SIZE_1;
    //m_features->payloadDataSize = POINT_SIZE::SIZE_1;
}

ImageDataItem::~ImageDataItem()
{

}

//设置长宽高
bool ImageDataItem::setup(int assWidth, int imgWidth, int height)
{
    if (assWidth <0 || imgWidth <= 0 || height <= 0)
        return false;
    std::shared_ptr<unsigned char> img[2]/*, ass[2]*/;
    for (int i=0; i<2; ++i)
    {
        img[i].reset(new unsigned char [imgWidth*height]);
        if (!img[i])
            return false;
    }

    //stop processing , reset image&assist data ptr, set features of width&height
	stop();
    for (int i=0; i<2; ++i)
    {
        m_dualImageBuffer[i] = img[i];
    }

    ////开始测试代码 //testing  初始化缓冲区1中的数据
    //unsigned char* buf = &(*img[1]);
    //for (int h=0; h<height; ++h)
    //{
    //    for (int w=0;w<imgWidth; ++w)
    //    {
    //        buf[h*imgWidth+w] = w;
    //    }
    //}
    //end testing

    //start processing
	IDataItem::setup(assWidth, imgWidth, height);
	start();

	return true;
}


void ImageDataItem::process()
{
    resetCounter();
    m_curFrameCnt = 0;

    unsigned char* buf = nullptr;
	int headerSize = sizeof(CMOS_FRAME_HEAD);
    int imgLineSize = m_features->payloadDataWidth;
    int assLineSize = m_features->attachedDataWidth;
    int lineSize = imgLineSize+assLineSize;
    
    std::shared_ptr<IBuffer>& inputBuffer_zero = m_inputBuffer[0].second;  //从缓冲区内取数据，然后去帧头

	////start testing
	//std::shared_ptr<IBuffer>& outputBuffer_zero = m_outputBuffer[0].second;
	//unsigned char *temp = new unsigned char[1280 * 1024]; 
	//int t=0;
	//for (int i = 0; i < 1280 * 1024; ++i)
	//{
	//	temp[i] = t;
	//	if (i % 5120 == 0) t++;
	//}
	////end test

	CMOS_FRAME_HEAD* pkgHeader = nullptr;
    while (m_processing)
    {

		////start testing
		//outputBuffer_zero->push_back(temp, m_features->linesPerFrame*imgLineSize);
		//std::this_thread::sleep_for(std::chrono::milliseconds(1));
		////end test
		
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
		if (FRM_SYNWORD != ntohl(pkgHeader->synWord))//检验帧同步字
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

//只要丢包就丢弃
#include <QFile>
//unsigned char* buf0 = new unsigned char[512 * 1024 * 1024];
//long long cnt = 0;
void ImageDataItem::storePayloadData(const unsigned char *buf)
{
	//面阵相机包格式处理流程：
	//提取帧计数（从0开始），检查行号，帧计数跳变后，行号应该从0开始，帧计数未跳变时，根据分通道检查图像数据
	//行号是否+1递增，正常则填入数据缓存相应的位置，错误则报错。未
	//满一帧但行号归0，报错，行标识不对，报错，包长度不对，报错。

	const CMOS_FRAME_HEAD* pkgHeader = reinterpret_cast<const CMOS_FRAME_HEAD*>(buf);//指向包头
	int headerSize = sizeof(CMOS_FRAME_HEAD);
	long long frmCnt = ntohl(pkgHeader->frmCount);//帧计数
	long long curLineCnt = ntohl(pkgHeader->lineCount);//行号

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
	int imgLineSize = m_features->payloadDataWidth;
	int assLineSize = m_features->attachedDataWidth;
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
			unsigned char *buffer = m_dualImageBuffer[0].get() + (m_features->linesPerFrame - curLineCnt)*imgLineSize;
			for (int i = 0; i < imgLineSize; ++i)
			{
				buffer[i] = buf[i + headerSize];  //buf->m_dualImageBuffer[0]
			}
			
			/*memcpy(buf0 + cnt, pkgHeader, headerSize);
			memcpy(buf0 + cnt + headerSize, buffer, lineSize);
			cnt += lineSize;

			if (cnt > 500 * 1024 * 1024)
			{
				cnt = 0;
				static int fnum = 0;
				QFile f(QObject::tr("C:/temp_%1.dat").arg(++fnum)); f.open(QIODevice::WriteOnly); f.write((char*)buf0, 500 * 1024 * 1024); f.close();
			}*/
		}

		if (curLineCnt == m_features->linesPerFrame - 1)//一帧的最后一行，将缓冲区[0]放入[1]中
		{
			resetCounter();  //reset and look for the first line of next img
			if (m_dualImageBuffer[0] && m_dualImageBuffer[1])
			{
				memcpy(m_dualImageBuffer[1].get(), m_dualImageBuffer[0].get(), m_features->linesPerFrame*imgLineSize);
			}
			std::shared_ptr<IBuffer>& outputBuffer_zero = m_outputBuffer[0].second;
			if (outputBuffer_zero)
			{
				outputBuffer_zero->push_back(m_dualImageBuffer[1].get(), m_features->linesPerFrame*imgLineSize);
			}
		}
	}
	else
	{
		resetCounter();  //reset and look for the first line of next img
	}
}

//不做丢包处理
//int temp[1024]{0};
//void ImageDataItem::storePayloadData(const unsigned char *buf)
//{
//	//面阵相机包格式处理流程：
//	//提取帧计数（从0开始），检查行号，帧计数跳变后，行号应该从0开始，帧计数未跳变时，根据分通道检查图像数据
//	//行号是否+1递增，正常则填入数据缓存相应的位置，错误则报错。未
//	//满一帧但行号归0，报错，行标识不对，报错，包长度不对，报错。
//
//	const CMOS_FRAME_HEAD* pkgHeader = reinterpret_cast<const CMOS_FRAME_HEAD*>(buf);//指向包头
//	int headerSize = sizeof(CMOS_FRAME_HEAD);
//	long long frmCnt = ntohl(pkgHeader->frmCount);//帧计数
//	long long curLineCnt = ntohl(pkgHeader->lineCount);//行号
//
//	int& lineCnt = m_curLineCnt;
//	int imgLineSize = m_features->payloadDataWidth;
//	int assLineSize = m_features->attachedDataWidth;
//	int lineSize = imgLineSize + assLineSize;
//
//	//帧计数未跳变，行号加1递增
//	//if (frmCnt == m_newFrame && (lineCnt + 1 == curLineCnt || -1 == lineCnt))
//	{
//		static int val = 0;
//		val++;
//		temp[val] = curLineCnt;
//		lineCnt = curLineCnt;
//		if (curLineCnt >= m_features->linesPerFrame)
//		{
//			//printf("\n>=(1024) lines");
//			return;
//		}
//		if (m_dualImageBuffer[0])
//		{
//			unsigned char *buffer = m_dualImageBuffer[0].get() + curLineCnt*imgLineSize;
//			for (int i = 0; i < imgLineSize; ++i)
//			{
//				buffer[i] = buf[imgLineSize - i + headerSize];  //buf->m_dualImageBuffer[0]
//			}
//		}
//
//		if (val >= 1024)//一帧的最后一行，将缓冲区[0]放入[1]中
//		{
//			val = 0;
//			resetCounter();  //reset and look for the first line of next img
//			if (m_dualImageBuffer[0] && m_dualImageBuffer[1])
//				memcpy(m_dualImageBuffer[1].get(), m_dualImageBuffer[0].get(), m_features->linesPerFrame*imgLineSize);
//		}
//	}
//}

//copy area of image data
bool ImageDataItem::copyArea(int x, int y, int w, int h, unsigned char* extPtr) const
{
	if (!m_dualImageBuffer[1] || !extPtr)
		return false;

	unsigned char* buffer = m_dualImageBuffer[1].get();
	for (int _h = 0; _h < h; ++_h) {
		memcpy(extPtr + _h*w, &(*buffer) + ((y + _h)*m_features->payloadDataWidth + x), w);
	}

	return true;
}

////get entire image data ptr
//bool ImageDataItem::internalImageDataPtr(const unsigned char*& ptr) const
//{
//	if (!m_dualImageBuffer[1] || !ptr)
//		return false;
//	ptr = m_dualImageBuffer[1].get();
//	return true;
//}
//
////get entire assist data ptr
//bool ImageDataItem::internalAssistDataPtr(const unsigned char*& ptr) const
//{
//	ptr = m_dualAssistBuffer[1].get();
//	return true;
//}



