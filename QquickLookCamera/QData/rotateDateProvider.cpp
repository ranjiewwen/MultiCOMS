#include "rotateDateProvider.h"


rotateDateProvider::rotateDateProvider()
{
	m_features->attachedDataSize = POINT_SIZE::SIZE_1;
	m_features->payloadDataSize = POINT_SIZE::SIZE_1;
}


rotateDateProvider::~rotateDateProvider()
{

}

bool rotateDateProvider::setup(int assWidth,int imgWidth,int height)
{
	if (assWidth <0 || imgWidth <= 0 || height <= 0)
		return false;
	std::shared_ptr<unsigned char> img;
	
	img.reset(new unsigned char[imgWidth*height*static_cast<int>(m_features->payloadDataSize)]);
	if (!img)
		return false;
	
	//stop processing , reset image&assist data ptr, set features of width&height
	IDataProcessUnit::stop();
	//need lock ?
	m_imageBuffer = img;

	//testing  初始化缓冲区1中的数据
	/*unsigned char* buf = &(*img);
	for (int h=0; h<height; ++h)
	{
	    for (int w=0;w<imgWidth; ++w)
	    {
	        buf[h*imgWidth+w] = w;
	    }
	}*/
	//end testing

	//start processing
	IDataItem::setup(assWidth, imgWidth, height);
	IDataProcessUnit::start();

	return true;
}

bool rotateDateProvider::copyArea(int x,int y,int w,int h,unsigned char *extPtr) const
{
	if (!m_imageBuffer || !extPtr)
		return false;

	unsigned char* buffer = m_imageBuffer.get();
	for (int _h = 0; _h < h; ++_h) {
		memcpy(extPtr + _h*w*static_cast<int>(m_features->payloadDataSize),
			&(*buffer) + ((y + _h)*m_features->payloadDataWidth + x) * static_cast<int>(m_features->payloadDataSize),
			w*static_cast<int>(m_features->payloadDataSize));
	}
	return true;
}

void rotateDateProvider::process()
{
	unsigned char* buf = nullptr;
	std::shared_ptr<IBuffer>& inputBuffer_zero = m_inputBuffer[0].second;
	while (m_processing)
	{
		if (!inputBuffer_zero)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
			continue;
		}
		//将整个数据包放入缓冲区buf
		if (0 != inputBuffer_zero->front(buf, 1280))//1280为有效数据长度
		{
			//printf("\nErr:将整个数据包放入缓冲区buf");
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
			continue;
		}
		int width=0, height=0;

       //完成图像数据旋转
		Rotate(m_imageBuffer.get(), buf,width,height,90);
	}

}

bool rotateDateProvider::internalImageDataPtr(const unsigned char*& ptr) const
{
	if (!m_dualImageBuffer[1] || !ptr)
		return false;
	ptr = m_dualImageBuffer[1].get();
	return true;
}

//get entire assist data ptr
bool rotateDateProvider::internalAssistDataPtr(const unsigned char*& ptr) const
{
	ptr = m_dualAssistBuffer[1].get();
	return true;
}

//24位真彩色图旋转angle°，图像高、宽改变，空白部分由白色填充
void rotateDateProvider::Rotate(unsigned char* dst, const unsigned char* src, int& width, int& height, int angle)
{
	int m_width = 1280, m_height = 1024;
	double angleRad = (double)angle / 180 * 3.1415926;
	double fcos = cos(angleRad);
	double fsin = sin(angleRad);
	width = m_width*abs(fcos) + m_height*abs(fsin);
	if (width % 4 != 0) width += 4 - width % 4;
	height = m_width*abs(fsin) + m_height*abs(fcos);
	if (height % 4 != 0) height += 4 - height % 4;
	memset(dst, 255, height * width * 3);
	int x = 0, y = 0, x0 = 0, y0 = 0;
	for (y0 = 0; y0 < m_height; ++y0)
	{
		for (x0 = 0; x0 < m_width; ++x0)
		{
			x = x0*fcos - y0*fsin - 0.5*m_width*fcos + 0.5*m_height*fsin + 0.5*width;
			y = x0*fsin + y0*fcos - 0.5*m_width*fsin - 0.5*m_height*fcos + 0.5*height;
			dst[(y*width + x) * 3 + 0] = src[(y0*m_width + x0) * 3 + 0];
			dst[(y*width + x) * 3 + 1] = src[(y0*m_width + x0) * 3 + 1];
			dst[(y*width + x) * 3 + 2] = src[(y0*m_width + x0) * 3 + 2];
		}
	}
	//return true;
}