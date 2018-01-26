#include "RotatedImageDataItem.h"
#include "ImageProcessing.h"

RotatedImageDataItem::RotatedImageDataItem(int _angle) :m_angle(_angle)
{
	
}


RotatedImageDataItem::~RotatedImageDataItem()
{
}

bool RotatedImageDataItem::changeAngle(int _angle)
{
	m_angle = _angle;
	stop();

	//求变换后的宽和高
	double angleRad = (double)m_angle / 180 * 3.1415926;
	double fcos = cos(angleRad);
	double fsin = sin(angleRad);
	int width = m_width*abs(fcos) + m_height*abs(fsin);
	if (width % 4 != 0) width += 4 - width % 4;
	int height = m_width*abs(fsin) + m_height*abs(fcos);
	if (height % 4 != 0) height += 4 - height % 4;
	std::shared_ptr<unsigned char> img[2];
	for (int i = 0; i < 2; ++i)
	{
		img[i].reset(new unsigned char[width*height]);
		if (!img[i])
			return false;
	}

	//stop processing , reset image&assist data ptr, set features of width&height
	stop();
	for (int i = 0; i < 2; ++i)
	{
		m_rotatedImageBuffer[i] = img[i];
	}


	//start processing
	IDataItem::setup(m_assWidth, width, height);
	start();

	return true;
}

//bool RotatedImageDataItem::setup(int assWidth, int imgWidth, int height)//辅助数据宽度，变换后的宽和高
//{
//	if (assWidth <0 || imgWidth <= 0 || height <= 0)
//		return false;
//	if (imgWidth % 4 != 0) 
//		imgWidth += 4 - imgWidth % 4;
//	if (height % 4 != 0) 
//		height += 4 - height % 4;
//
//	std::shared_ptr<unsigned char> img[2];
//	for (int i = 0; i<2; ++i)
//	{
//		img[i].reset(new unsigned char[imgWidth*height]);
//		if (!img[i])
//			return false;
//	}
//
//	//stop processing , reset image&assist data ptr, set features of width&height
//	stop(); 
//	for (int i = 0; i<2; ++i)
//	{
//		m_rotatedImageBuffer[i] = img[i];
//	}
//
//	//旋转90，则高宽互换
//	double angleRad = (double)m_angle / 180 * 3.1415926;
//	double fcos = fabs(cos(angleRad));
//	double fsin = fabs(sin(angleRad));
//	m_height = abs((imgWidth*fsin - height*fcos) / (fsin*fsin - fcos*fcos));
//	m_width = abs((imgWidth*fcos - height*fsin) / (fcos*fcos - fsin*fsin));
//	if (m_width % 4 != 0)
//		m_width += 4 - m_width % 4;
//	if (m_height % 4 != 0)
//		m_height += 4 - m_height % 4;
//
//	//start processing
//	IDataItem::setup(assWidth, imgWidth, height);
//	start();
//
//	return true;
//}

//辅助数据宽度，变换前的宽和高
bool RotatedImageDataItem::setup(int _assWidth, int _imgWidth, int _height)
{
	if (_assWidth <0 || _imgWidth <= 0 || _height <= 0)
		return false;

	m_height = _height;
	m_width = _imgWidth;
	m_assWidth = _assWidth;

	//求变换后的宽和高
	double angleRad = (double)m_angle / 180 * 3.1415926;
	double fcos = cos(angleRad);
	double fsin = sin(angleRad);
	int width = m_width*abs(fcos) + m_height*abs(fsin);
	if (width % 4 != 0) width += 4 - width % 4;
	int height = m_width*abs(fsin) + m_height*abs(fcos);
	if (height % 4 != 0) height += 4 - height % 4;

	std::shared_ptr<unsigned char> img[2];
	for (int i = 0; i < 2; ++i)
	{
		img[i].reset(new unsigned char[width*height]);
		if (!img[i])
			return false;
	}

	//stop processing , reset image&assist data ptr, set features of width&height
	stop();
	for (int i = 0; i < 2; ++i)
	{
		m_rotatedImageBuffer[i] = img[i];
	}


	//start processing
	IDataItem::setup(_assWidth, width, height);
	start();

	return true;
}
void RotatedImageDataItem::process()
{
	std::shared_ptr<IBuffer>& inputBuffer_zero = m_inputBuffer[0].second;
	std::shared_ptr<IBuffer>& outputBuffer_zero = m_outputBuffer[0].second;
	unsigned char *buffer = new unsigned char[m_height*m_width];
	
	while (m_processing)
	{
		if (!inputBuffer_zero)//若inputBuffer没有被注册
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
			continue;
		}
		if (m_rotatedImageBuffer[0] && m_rotatedImageBuffer[1])
		{
			if (0 != inputBuffer_zero->front(buffer, m_height*m_width))
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
				continue;
			}
			Rotate(m_rotatedImageBuffer[0].get(), buffer, m_angle);
			memcpy(m_rotatedImageBuffer[1].get(), m_rotatedImageBuffer[0].get(), m_features->linesPerFrame*m_features->payloadDataWidth);
			inputBuffer_zero->pop_front(m_height*m_width);
			if (outputBuffer_zero)
			{
				outputBuffer_zero->push_back(m_rotatedImageBuffer[1].get(), m_features->linesPerFrame*m_features->payloadDataWidth);
			}
		}
	}
	
}

//copy area of image data
bool RotatedImageDataItem::copyArea(int x, int y, int w, int h, unsigned char* extPtr) const
{
	if (!m_rotatedImageBuffer || !extPtr)
		return false;

	unsigned char* buffer = m_rotatedImageBuffer[1].get();
	for (int _h = 0; _h < h; ++_h) {
		memcpy(extPtr + _h*w, &(*buffer) + ((y + _h)*m_features->payloadDataWidth + x), w);
	}

	return true;
}

//24位真彩色图旋转angle°，图像高、宽改变，空白部分由白色填充
//void RotatedImageDataItem::Rotate(unsigned char* dst, const unsigned char* src, int width, int height, int angle)
//{
//	double angleRad = (double)angle / 180 * 3.1415926;
//	double fcos = cos(angleRad);
//	double fsin = sin(angleRad);
//	width = m_width*abs(fcos) + m_height*abs(fsin);
//	if (width % 4 != 0) width += 4 - width % 4;
//	height = m_width*abs(fsin) + m_height*abs(fcos);
//	if (height % 4 != 0) height += 4 - height % 4;
//	memset(dst, 255, height * width * 3);
//	int x = 0, y = 0, x0 = 0, y0 = 0;
//	for (y = 0; y < height; ++y)
//	{
//		for (x = 0; x < width; ++x)
//		{
//			/*x = x0*fcos - y0*fsin - 0.5*m_width*fcos + 0.5*m_height*fsin + 0.5*width;
//			y = x0*fsin + y0*fcos - 0.5*m_width*fsin - 0.5*m_height*fcos + 0.5*height;*/
//			x0 = x*fcos + y*fsin - 0.5*width*fcos - 0.5*height*fsin + 0.5*m_width;
//			y0 = -x*fsin + y*fcos + 0.5*width*fsin - 0.5*height*fcos + 0.5*m_height;
//			if (0 <= x0&&x0 < m_width && 0 <= y0&&y0 < m_height)
//			{
//				dst[(y*width + x) * 3 + 0] = src[(y0*m_width + x0) * 3 + 0];
//				dst[(y*width + x) * 3 + 1] = src[(y0*m_width + x0) * 3 + 1];
//				dst[(y*width + x) * 3 + 2] = src[(y0*m_width + x0) * 3 + 2];
//			}
//		}
//	}
//}

//8位灰色图旋转angle°，图像高、宽改变，空白部分由白色填充
void RotatedImageDataItem::Rotate(unsigned char* dst, const unsigned char* src, int angle)
{
	double angleRad = (double)angle / 180 * 3.1415926;
	double fcos = cos(angleRad);
	double fsin = sin(angleRad);
	int height = m_features->linesPerFrame;
	int width = m_features->payloadDataWidth;
	memset(dst, 255, height * width);
	int x = 0, y = 0, x0 = 0, y0 = 0;
	for (y = 0; y < height; ++y)
	{
		for (x = 0; x < width; ++x)
		{
			/*x = x0*fcos - y0*fsin - 0.5*m_width*fcos + 0.5*m_height*fsin + 0.5*width;
			y = x0*fsin + y0*fcos - 0.5*m_width*fsin - 0.5*m_height*fcos + 0.5*height;*/
			x0 = x*fcos + y*fsin - 0.5*width*fcos - 0.5*height*fsin + 0.5*m_width;
			y0 = -x*fsin + y*fcos + 0.5*width*fsin - 0.5*height*fcos + 0.5*m_height;
			if (0 <= x0&&x0 < m_width && 0 <= y0&&y0 < m_height)
			{
				dst[(y*width + x)] = src[(y0*m_width + x0)];
			}
		}
	}
}
