#pragma once

#include "IDataProcessUnit.h"
#include "IDataItem.h"

class RotatedImageDataItem : public IDataItem, public IDataProcessUnit
{
protected:
	int m_height;//变换前的高，m_features保存变换后的宽和高
	int m_width;//变换前的宽，m_features保存变换后的宽和高
	int m_assWidth;
	int m_angle;
	std::shared_ptr<unsigned char> m_rotatedImageBuffer[2]; //变换后的图像数据,双缓冲
public:
	RotatedImageDataItem(int _angle);
	~RotatedImageDataItem();
	bool changeAngle(int _angle);
	//inherit from IDataItem, 变换之前的宽和高
	virtual bool setup(int assWidth, int imgWidth, int height) override;
	//copy area of image data
	virtual bool copyArea(int x, int y, int w, int h, unsigned char* extPtr) const override;
protected:
	//inherit from IDataProcessUnit
	virtual void process() override;
	//顺时针旋转一定角度
	void Rotate(unsigned char* dst, const unsigned char* src, int angle);
};

