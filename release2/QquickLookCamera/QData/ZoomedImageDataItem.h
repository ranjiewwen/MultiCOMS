#pragma once
#include "IDataProcessUnit.h"
#include "IDataItem.h"

class ZoomedImageDataItem : public IDataItem, public IDataProcessUnit
{
protected:
	int m_height;//变换前的高，m_features保存变换后的宽和高
	int m_width;//变换前的宽，m_features保存变换后的宽和高
	float m_ratio;
	std::shared_ptr<unsigned char> m_zoomedImageBuffer[2]; //变换后的图像数据,双缓冲
public:
	ZoomedImageDataItem(float _ratio);
	~ZoomedImageDataItem();
	//inherit from IDataItem, 变换之前的宽和高
	virtual bool setup(int assWidth, int imgWidth, int height) override;
	//copy area of image data
	virtual bool copyArea(int x, int y, int w, int h, unsigned char* extPtr) const override;
protected:
	//inherit from IDataProcessUnit
	virtual void process() override;
	//放大ratio倍
	void Zoom(unsigned char* dst, const unsigned char* src, float _ratio);
};

