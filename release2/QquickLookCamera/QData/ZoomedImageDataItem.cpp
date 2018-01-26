#include "ZoomedImageDataItem.h"


ZoomedImageDataItem::ZoomedImageDataItem(float _ratio) :m_ratio(_ratio)
{
}


ZoomedImageDataItem::~ZoomedImageDataItem()
{
}

//辅助数据宽度，变换前的宽和高
bool ZoomedImageDataItem::setup(int _assWidth, int _imgWidth, int _height)
{
	if (_assWidth < 0 || _imgWidth <= 0 || _height <= 0)
		return false;

	m_height = _height;
	m_width = _imgWidth;

	//求变换后的宽和高
	int width = m_width*m_ratio;
	if (width % 4 != 0) width += 4 - width % 4;
	int height = m_height*m_ratio;
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
		m_zoomedImageBuffer[i] = img[i];
	}


	//start processing
	IDataItem::setup(_assWidth, width, height);
	start();

	return true;
}

void ZoomedImageDataItem::process()
{
	std::shared_ptr<IBuffer>& inputBuffer_zero = m_inputBuffer[0].second;
	unsigned char *buffer = new unsigned char[m_height*m_width];

	while (m_processing)
	{
		if (!inputBuffer_zero)//若inputBuffer没有被注册
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
			continue;
		}
		if (m_zoomedImageBuffer[0] && m_zoomedImageBuffer[1])
		{
			if (0 != inputBuffer_zero->front(buffer, m_height*m_width))
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
				continue;
			}
			Zoom(m_zoomedImageBuffer[0].get(), buffer, m_ratio);
			memcpy(m_zoomedImageBuffer[1].get(), m_zoomedImageBuffer[0].get(), m_features->linesPerFrame*m_features->payloadDataWidth);
			inputBuffer_zero->pop_front(m_height*m_width);
		}
	}

}

//copy area of image data
bool ZoomedImageDataItem::copyArea(int x, int y, int w, int h, unsigned char* extPtr) const
{
	if (!m_zoomedImageBuffer || !extPtr)
		return false;

	unsigned char* buffer = m_zoomedImageBuffer[1].get();
	for (int _h = 0; _h < h; ++_h) {
		memcpy(extPtr + _h*w, &(*buffer) + ((y + _h)*m_features->payloadDataWidth + x), w);
	}

	return true;
}

//8位hui色图缩放，ratio为缩放因子（ratio=2即为放大2倍）
void ZoomedImageDataItem::Zoom(unsigned char* dst, const unsigned char* src, float _ratio)
{
	int x = 0, y = 0, x0 = 0, y0 = 0;
	for (y = 0; y < m_features->linesPerFrame; ++y)
	{
		for (x = 0; x < m_features->payloadDataWidth; ++x)
		{
			x0 = x / _ratio;
			y0 = y / _ratio;
			if (0 <= x0&&x0 < m_width && 0 <= y0&&y0 < m_height)
			{
				dst[(y*m_features->payloadDataWidth + x)] = src[(y0*m_width + x0)];
			}
		}
	}
}

