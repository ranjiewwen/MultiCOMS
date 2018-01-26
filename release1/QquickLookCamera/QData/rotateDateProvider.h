#ifndef ROTATE_DATE_PROVIDER_H
#define ROTATE_DATE_PROVIDER_H
//#pragma once
#include "..\QData\IDataProcessUnit.h"
#include "../QData/IDataItem.h"
class rotateDateProvider :
	public IDataItem, public IDataProcessUnit
{
protected:
	//bool m_Buffer{false};
	std::shared_ptr<unsigned char> m_imageBuffer;
	std::shared_ptr<unsigned char> m_dualImageBuffer[2]; //entire image data buffer, dual
	std::shared_ptr<unsigned char> m_dualAssistBuffer[2]; //entire assist data buffer, dual,6+8=14bytes frame header
public:
	virtual bool setup(int assWidth,int imgWidth,int height) override;
	virtual bool copyArea(int x,int y,int w,int h,unsigned char *extPtr)const override;
	//get entire image data ptr
	virtual bool internalImageDataPtr(const unsigned char*& ptr) const override;
	//get entire assist data ptr
	virtual bool internalAssistDataPtr(const unsigned char*& ptr) const override;
public:
	rotateDateProvider();
	~rotateDateProvider();
protected:
	virtual void process() override; //inherit from IProcessingItem
	void Rotate (unsigned char* dst, const unsigned char* src, int& width, int& height, int angle);
};

#endif